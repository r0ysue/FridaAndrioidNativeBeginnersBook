#!/usr/bin/env python
# -*- coding: utf-8 -*-

from idaapi import *
import codecs
import time
import os


def chunks(l, n):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        yield l[i:i + n]

# 加载数据库
# 我觉得必须得这样，我那种很丑陋，很不优雅，而且扩展性很差
def load_signatures():
    import xml.etree.ElementTree as ET

    db = idadir("plugins/hashsrch.xml")
    if not os.path.isfile(db):
        db = os.path.join(get_user_idadir(), "plugins/hashsrch.xml")
    root = ET.parse(db).getroot()

    signature = []
    for p in root:
        # <p t="name [64.le rev.64&amp;]">
        name, data = p.attrib['t'].split(" [")
        bits, endian, size = data[:-1].split(".")

        if "&" in size:
            if bits == "float":
                bits = 32
            elif bits == "double":
                bits = 64
            else:
                bits = int(bits)

        signature.append({
            "name": name,
            "bits": bits,
            "endian": endian,
            "size": size,
            "data": codecs.decode(p.text, ('hex')),
        })

    return signature


class Chooser(Choose):
    def __init__(self, items):
        Choose.__init__(self, "Signsrch", [["Address", 20], ["Label", 80]], embedded=False)
        self.items = items
        self.icon = 160

    def GetItems(self):
        return self.items

    def SetItems(self, items):
        self.items = [] if items is None else items

    def OnClose(self):
        pass

    def OnGetLine(self, n):
        addr, label = self.items[n]
        seg_name = get_segm_name(getseg(addr))
        if seg_name:
            return ["%s:%X" % (seg_name, addr), label]
        else:
            return ["unknown:%X" % addr, label]

    def OnGetSize(self):
        return len(self.items)

    def OnSelectLine(self, n):
        jumpto(self.items[n][0])

# 日噢，突然想到，hook监控变量确实很帅，但这只应该作为一种补充（因为通过指针，比如openssl那样的调用，确实xref找不到）通过idc脚本hook上游的函数才是正道
# 对于正则匹配的那些，函数，确实做到了，但我想像signsrch那样，对静态的特征常量进行检索并生成frida脚本，其实就不太够了，应该这样：
# 多线并发
# 正则匹配的idamagic ，这是针对初始化连接变量的，作为frida 的一个单独函数
# 方式二，检测变量访问，这样可以得到两个信息，1是用了so中哪些加密方法，2是掉用处 缺点是如果多次调用，只显示第一次以及缺少调用栈的支持
# 方法三：ida xref hook所有上游的函数，有点是有调用栈，而且比较全面，缺点是如果通过指针偏移调用，xref找不到。

# 相比较signsrch
# 1.我们不做那么多算法
# 2.不用考虑大小端序，android就是小端序
class signsrch_t(plugin_t):
    flags = PLUGIN_PROC
    comment = "Signsrch"
    help = ""
    wanted_name = "Signsrch"
    wanted_hotkey = ""

    def init(self):
        print("Signsrch (Python Version) (v1.0) plugin has been loaded.")
        return PLUGIN_OK

    def run(self, arg):
        ignored = ["be", "le"][cvar.inf.is_be()]
        signatures = [s for s in load_signatures() if s["endian"] != ignored]

        if not signatures:
            print("No signature loaded, Aborted")
            return
        print("%d signatures loaded" % len(signatures))

        # Scan every segments
        start_time = time.time()

        found = []
        # 这儿也可以学习，正规化嘛
        for i in range(get_segm_qty()):
            seg = getnseg(i)
            seg_name = get_segm_name(seg)
            seg_class = get_segm_class(seg)
            if seg.type in (SEG_XTRN, SEG_GRP, SEG_NULL, SEG_UNDF, SEG_ABSSYM, SEG_COMM, SEG_IMEM,):
                print("Skipping segment: %s, %s" % (seg_name, seg_class))
                continue

            print("Processing segment: %s, %s, 0x%08X - 0x%08X, %dbytes " % (seg_name, seg_class, seg.start_ea, seg.end_ea, seg.size()))
            bytes = get_bytes(seg.start_ea, seg.size())
            for sig in signatures:
                ea = None
                if "&" in sig["size"]:
                    bits = sig["bits"]
                    idx = 0
                    for s in chunks(sig["data"], bits // 8):
                        idx = bytes.find(s, idx)
                        if idx == -1:
                            ea = None
                            break

                        if ea == None:
                            ea = seg.start_ea + idx

                        idx += bits // 8
                else:
                    idx = bytes.find(sig["data"])
                    if idx != -1:
                        ea = seg.start_ea + idx

                if ea != None: # found
                    name = sig["name"]
                    found.append([ea, name])
                    print("Found %s @ 0x%X" % (name, ea))

                    # Add comment
                    cmt = get_cmt(ea, True)
                    if not cmt:
                        set_cmt(ea, '<Signsrch> "%s"' % name, True)
                    elif "Signsrch" not in cmt:
                        set_cmt(ea, cmt + ' <Signsrch> "%s"' % name, True)

        print("Found %d matches in %s seconds" % (len(found), time.time() - start_time))

        if found:
            ch = Chooser(found)
            ch.Show()

    def term(self):
        pass

def PLUGIN_ENTRY():
    return signsrch_t()

# 其实这个脚本挺简单的，甚至挺简陋的。。。

