# 导入Unicorn
import inspect
import operator
import os
import platform
import subprocess

import lief as lief
import unicorn
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, arm_const, UC_MODE_THUMB, \
    UC_QUERY_MODE
# 导入capstone
import capstone as cs
# 导入解析和处理so的第三方库,elftools和lief都是合适的选择，但lief封装程度更高，可以带来更简约优雅的代码，作为一个demo项目，我们选择lief。
# import lief
# 这个库可以不导入的。
import math
# pygments，格式化显示和输出，突出一个优雅
from pygments import highlight
from pygments.formatters.terminal import TerminalFormatter as formatter
from pygments.lexers.asm import NasmLexer
# 给输出增添颜色
import colorama
# hexdump
import hexdump


## 一个小而美的unicorn trace框架，
## 代码可读性很高，小而美。
## 参考了qiling，unidbg，androidmativeemu，rainbow的代码。


class uTrace:
    def __init__(self):
        # 初始化一台模拟器实例
        self.emu = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        # 页大小,4KB的保守且稳定的选择。
        self.page_size = 0x1000
        # 分配100M的虚拟内存给二进制文件
        self.map_space(0, 0xA00000)

        # 栈的设置
        self.STACK_ADDR = 0xE00000
        self.map_space(self.STACK_ADDR - 0x2000, self.STACK_ADDR + 320)

        # 外部传入数据的存放
        # data segment
        self.DATA_ADDR = 0xB00000
        # JNI 存放位置
        self.JNI_ADDR = 0xB50000
        self.JNI_ADDR_FUC = 0xB60000

        self.map_space(self.DATA_ADDR, self.DATA_ADDR + 0x100000)

        # HEAP
        self.HEAP_ADDR = 0xC1000000
        self.map_space(self.HEAP_ADDR, self.HEAP_ADDR + 0x100000)
        self.block = 0x100

        # 最大执行指令条数
        #  if given, is the maximum number of instructions to run.If None, the number is unlimited.
        self.count = 300000

        # 寄存器们
        self.INTERNAL_REGS = ["r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                              "pc", "lr"]
        known_regs = [i[len('UC_ARM_REG_'):] for i in dir(unicorn.arm_const) if
                      '_REG' in i]
        self.reg_map = {r.lower(): getattr(unicorn.arm_const, 'UC_ARM_REG_' + r)
                        for r in known_regs}

        # 反汇编引擎
        self.disasm = cs.Cs(cs.CS_ARCH_ARM, cs.CS_MODE_ARM)
        self.disasm.detail = True

        # 模式，16为thumb，0是arm。
        # 默认是thumb，运行时动态调整模式。
        # cpsr（r16）寄存器的状态可以反映thumb or arm，或者根据地址的奇偶，unicorn封装的类使用奇偶，我们听它的吧。
        self.mode = 16

        # trace开关
        self.trace = True
        self.trace_regs = True
        self.mem_trace = True
        self.reg_leak = None

        # 添加hook&trace
        self.setup()

        # 断点调试
        self.breakpoints = []
        self.memory_breakpoints = []

        # 输出格式
        ## Prepare a live disassembler
        self.asm_hl = NasmLexer()
        self.asm_fmt = formatter(outencoding="utf-8")

        colorama.init()

        ## 函数hook以及处理
        self.functions = {}
        self.functionsNames = []
        self.stubbed_functions = {}
        self.inner_stubbed()

        # JNI 处理
        self.jni_functions = {}
        self.jni_functions_names = []
        self.jni_functions_implement = {}
        self.inner_jni()

        # 重置信息
        self.reset()

    # SO加载器
    def load(self, filename, verbose=False):
        self.load_module(filename, verbose)

    # 开始运行,设置最多执行100w条指令，防止由于样本问题或者utrace自身的bug陷入死循环。
    def start(self, begin, end, timeout=0):
        return self.emu.emu_start(begin, end, timeout, self.count)



    # 重置模拟器参数
    def reset(self):
        self.trace_reset()
        self.heap_reset()
        self.stack_reset()

    # 批量设置寄存器的值
    def set_reg(self, **kwargs):
        for name, value in kwargs.items():
            self[name] = value

    # 读取全部寄存器的值
    def get_reg(self):
        """Get the values of multiple registers."""
        return tuple(hex(self[name]) for name in self.INTERNAL_REGS)


    @property
    def pc(self):
        """The current program counter."""
        return hex(self["pc"])

    @pc.setter
    def pc(self, value):
        self["pc"] = value

    @property
    def sp(self):
        """The current stack pointer."""
        return hex(self["sp"])

    @sp.setter
    def sp(self, value):
        self["sp"] = value

    @property
    def r0(self):
        return hex(self["r0"])

    @r0.setter
    def r0(self, value):
        self["r0"] = value

    @property
    def r1(self):
        return hex(self["r1"])

    @r1.setter
    def r1(self, value):
        self["r1"] = value

    @property
    def r2(self):
        return hex(self["r2"])

    @r2.setter
    def r2(self, value):
        self["r2"] = value

    @property
    def r3(self):
        return hex(self["r3"])

    @r3.setter
    def r3(self, value):
        self["r3"] = value

    # 栈重置
    def stack_reset(self):
        self.emu.reg_write(arm_const.UC_ARM_REG_SP, self.STACK_ADDR)

    # 寄存器trace重置
    def trace_reset(self):
        self.reg_leak = None
        for reg in self.INTERNAL_REGS:
           self[reg] = 0


    # 堆重置
    def heap_reset(self):
        self.HEAP_ADDR = 0xC100000

    # SO加载器实现
    def load_module(self, filename, verbose):
        # 第一步：加载so文件进模拟器虚拟内存中
        # 使用大名鼎鼎的elftools对so文件进行解析
        with open(filename, "rb")as sobinary:
            elf = lief.parse(sobinary)
        # SO的加载和运行属于执行视图
        # 规则：只需要根据段头表的引导，加载类型为load的段，其余段无需加载。

        for seg in elf.segments:
            if seg.type == lief.ELF.SEGMENT_TYPES.LOAD:
                # physical_size 挺适合这里的需求,先跑起来吧，唉
                paddr = seg.virtual_address
                self.mem_write(paddr, bytes(seg.content))

        # 加载Linker，各个基于Unicorn的模拟执行框架也都大同小异。
        # 其实对于unicorn和模拟执行的需求而言，可能并不用这样，甚至都不应该用虚拟地址，而是用实际地址
        # 这并不是水平差或者粗鲁，而是基于unicorn构建的模拟执行工具像那玩意多过像操作系统。
        # 直接加载进内存，不考虑执行权限。（想到一个情况，日噢，unidbg不会支持一份代码多个拷贝吧？如果那样子的话
        # unidbg真的更像操作系统呢。。。）
        #
        # 处理重定向
        # 使得GOT表中SO内部的函数可以顺利跳转
        # TODO: SECTION 被抹掉的情况下，LIEF可以顺利加载relocation吗？不知道它是不是通过section header获取的这些。
        for r in elf.relocations:
            if r.symbol.is_function:
                # print(r.symbol.name)
                if r.symbol.value != 0:
                    self.mem_write(r.address, r.symbol.value.to_bytes(4,
                                                                      byteorder='little'))
                    # 这儿为什么减1呢？
                    self.functions[r.symbol.value - 1] = r.symbol.name
                    self.functionsNames.append(r.symbol.name)
                else:
                    self.mem_write(r.address,r.address.to_bytes(4, byteorder='little'))
                    self.functions[r.address] = r.symbol.name
                    self.functionsNames.append(r.symbol.name)

        for f in elf.exported_functions:
            name = f.name
            if name not in self.functionsNames:
                self.functionsNames.append(name)
                self.functions[f.address - 1] = name


    def setup(self):
        # 函数 hook
        self.block_hook = self.emu.hook_add(unicorn.UC_HOOK_BLOCK,self.block_handler)
        # 逐条 code
        self.code_hook = self.emu.hook_add(unicorn.UC_HOOK_CODE,self.code_trace)
        # 内存 hook
        self.mem_access_hook = self.emu.hook_add(unicorn.UC_HOOK_MEM_READ | unicorn.UC_HOOK_MEM_WRITE,self.trace_mem)
        # 内存 读写异常hook
        self.mem_unmapped_hook = self.emu.hook_add(unicorn.UC_HOOK_MEM_UNMAPPED, self.unmapped_hook)

    # 函数级hook
    def block_handler(self, uci, address, size, user_data):
        # 根据CPSR寄存器判断模式
        # thumb_bit = self.emu.reg_read(unicorn.arm_const.UC_ARM_REG_CPSR) & 0x20
        # if thumb_bit == 0:
        #     self.disasm.mode = cs.CS_MODE_ARM
        # else:
        #     self.disasm.mode = cs.CS_MODE_THUMB

        self.base_block_handler(address)

    def base_block_handler(self, address):
        """
        //TODO 实现函数的Hook和替换
        """
        # 如果地址是导入导出表中的函数，显式打印
        if address in self.functions.keys():
            name = self.functions[address]
            ## name demangle
            true_name = name_filt(name)
            if true_name != name:
                print(
                    f"\n {self.color('MAGENTA', f'{name}[{true_name}]')}(...) @ 0x{address:x}",
                    end=" ")
            else:
                print(
                    f"\n {self.color('MAGENTA', f'{name}')}(...) @ 0x{address:x}",
                    end=" ")
        # else:
        #     print(
        #         f"\n {self.color('MAGENTA', 'sub_' + hex(address).replace('0x', ''))}(...)",
        #         end=" ")

        # 如果一个地址是导入导出表的函数，同时又在需要被hook替换的名单中
        if address in self.functions.keys():
            name = self.functions[address]
            if name in self.stubbed_functions:
                self.stubbed_functions[name](self)
                self.return_force()
                # print(f"\n {self.color('MAGENTA', '↑↑↑Realized by user')}",
                #       end=" ")

        # 替换一个未导出函数
        if address in self.stubbed_functions:
            self.stubbed_functions[address](self)
            self.return_force()
            print(f"\n {self.color('MAGENTA', '↑↑↑Realized by user')}", end=" ")

        if address in self.jni_functions.keys():
            index = self.jni_functions[address]
            name = self.jni_functions_names[index]
            #  TODO 这部分写的太烂了
            # if name in self.jni_functions_implement:
            self.return_force()
            print(f"\n {self.color('MAGENTA', f'{name}')}(...)", end= " ")

    def code_trace(self, uci, address, size, data):
        # trace的良好实现依赖于capstone 牛逼哄哄的 regs_access。
        """
        Capstone 4's 'regs_access' method is used to find out which registers are explicitly modified by an instruction.
        Once found, the information iss stored in self.reg_leak to be stored at the next instruction, once the unicorn engine actually performed the current instruction.
        """
        # hit breakpoint

        # 采用官方的方式
        self.mode = self.emu.query(UC_QUERY_MODE)
        self.disasm.mode = self.mode
        if address in self.breakpoints:
            print(f"\n*** Breakpoint hit at 0x{address:x} ***")
            self.dump_reg()

            # 调试器死循环逻辑，通过无名侠来优化，需要添加更多。
            # 至少再来个readpointer()吧？
            # 再其次也得按照frida的标准搞一下？吧
            while True:
                s = input("Press Enter to continue：")
                # or Input \"hexdump addr size\" to display an address:

                if s == '':
                    break
                try:
                    self.self_dbg(s)

                except Exception as e:
                    print("Error:", e)
                    continue

        # if say need trace,default is True
        if self.trace:
            # print changed reg value
            # 不懂为什么叫寄存器泄露（漏洞）
            if self.reg_leak is not None:
                for x in self.reg_leak[1]:
                    print(f" {x} = {self[x]:08x} ", end="")

            # find and save the changed reg and print it on next asm
            # 生成细节（分析对寄存器的显式隐式读写）会占用更多内存，使内部操作复杂化，并使引擎稍微慢一点，但我们不在乎。
            if self.trace_regs:
                ins = self.disassemble_single_detailed(address, size)
                # Return (list-of-registers-read, list-of-registers-modified) by this instructions.
                # This includes all the implicit & explicit registers.
                regs_read, regs_written = ins.regs_access()
                if len(regs_written) > 0:
                    self.reg_leak = (
                    address, list(map(ins.reg_name, regs_written)))
                else:
                    self.reg_leak = None



                op_bytes = bytes(ins.bytes).hex()
                # print asm
                self.print_asmline(address, self.mode, op_bytes, ins.mnemonic, ins.op_str)
            else:
                # if not want to use capstone access reg yo trace reg,use a simple and lite api "disassemble_single".

                adr, size, ins, op_str = self.disassemble_single(address, size)
                op_bytes = bytes(ins.bytes).hex()
                self.print_asmline(adr, self.mode, op_bytes, ins, op_str)

        instruction = self[address]
        if instruction == "0x0" :
            uci.emu_stop()
            error = "你走到了没有汇编的荒原!"
            raise Exception(error)

    def unmapped_hook(self, uci, access, address, size, value, user_data):
        # uci.mem_map((address//(0x1000))*0x1000, 0x1000)
        """ Warns whereas the unicorn engine stopped on an unmapped access """
        uci.emu_stop()
        error = f"Unmapped fetch at {hex(address)}(Emu stopped in {uci.reg_read(self['pc']):x})"
        raise Exception(error)


    # uc.query(UC_QUERY_MODE) 可感知当前的状况吗，需要验证// TODO
    def skip_current_instruction(self):
        pc = self["pc"]
        if self.emu.query(UC_QUERY_MODE) == UC_MODE_THUMB:
            pc += 2
        else:
            pc += 4
        self["pc"] = pc

    # 命令行调试器
    def self_dbg(self, s):
        command = s.split(" ")[0]
        if command not in ["hexdump", "readPointer", "readCString"]:
            print("请检查拼写，当前只支持如下指令")
            print("hexdump address size : hexdump 0x100 0x20")
            print("readPointer address : readPointer 0x1000")
            print("readCString address : readCString 0xC24")
        if command == "hexdump":
            address, size = s.split(" ")[1:]
            self.hexdump(address, size)
        elif command == "readPointer":
            address = eval(s.split(" ")[1])
            print(self[address])
        elif command == "readCString":
            address = eval(s.split(" ")[1])
            print(self.readCString(address))

    def trace_mem(self, uci, access, address, size, value, user_data):
        """ Hook that shows a visual trace of memory accesses in the form '[address written to] <- value written' or 'value read <- [address read]' """
        if self.mem_trace:
            addr = self.color("BLUE", f"0x{address:08x}")
            if access == unicorn.UC_MEM_WRITE:
                val = self.color("CYAN", f"{value:x}")
                print(f"  [{addr}] <- {val} ", end=" ")
                if address in self.memory_breakpoints:
                    print("{:30}  ---=== {} ===---".format(" ", "WRITE"))
                    data = uci.mem_read(address, 0x100)
                    print(self.advance_dump(data, address))
            else:
                val = int.from_bytes(uci.mem_read(address, size), "little")
                val = self.color("CYAN", f"{val:8x}")
                print(f"  {val} <- [{addr}]", end=" ")
                if address in self.memory_breakpoints:
                    print("{:30}  ---=== {} ===---".format(" ", "READ"))
                    data = uci.mem_read(address, 0x100)
                    print(self.advance_dump(data, address))


    # 这个内存异常回调比rainbow的广
    def hook_mem_invalid(self, uc, access, address, size, value, user_data):
        access_memory_str = ""
        if access == UC_MEM_READ_UNMAPPED:
            print("!!! Invalid Memory is being READ at 0x%x, data size = %u" % (address, size))
            access_memory_str = "UC_MEM_READ_UNMAPPED"
        elif access == UC_MEM_WRITE_UNMAPPED:
            access_memory_str = "UC_MEM_WRITE_UNMAPPED"
        elif access == UC_MEM_FETCH_PROT:
            access_memory_str = "UC_MEM_FETCH_PROT"
        elif access == UC_MEM_WRITE_PROT:
            access_memory_str = "UC_MEM_WRITE_PROT"
        elif access == UC_MEM_READ_PROT:
            access_memory_str = "UC_MEM_READ_PROT"

        print("!!! Invalid Memory is being at 0x%x, data size = %u" % (address, size))
        print (access_memory_str)
        # pdb.set_trace()
        self.last_error = "UC_HOOK_MEM_INVALID at 0x%x access %d" % (address, access)
        print (self.last_error)
        return False

    # 设置断点
    def add_bkpt(self, address):
        if address not in self.breakpoints:
            self.breakpoints += [address]

    # 设置内存断点
    def add_memory_bkpt(self, address):
        if address not in self.memory_breakpoints:
            self.memory_breakpoints += [address]

    # print all reg
    def dump_reg(self):
        """ Dumps all regs when a breakpoint is hit """
        for reg in self.INTERNAL_REGS:
            print(f"{reg} : {self[reg]:x}")


    def hexdump(self, addr, size):
        if isinstance(addr,str):
            addr = eval(addr)
        if isinstance(size,str):
            size = eval(size)
        data = bytes(self[addr:addr + size])
        ret = self.advance_dump(data, addr)
        print(ret)

    # 默认hexdump方法索引总是从0开始。
    def advance_dump(self, data, base):
        generator = hexdump.genchunks(data, 16)
        retstr = ''
        for addr, d in enumerate(generator):
            # 使展示的索引从base开始
            line = '%08X: ' % (base + addr * 16)
            dumpstr = hexdump.dump(d)
            line += dumpstr[:8 * 3]
            if len(d) > 8:  # insert separator if needed
                line += ' ' + dumpstr[8 * 3:]
            # ................
            # calculate indentation, which may be different for the last line
            pad = 2
            if len(d) < 16:
                pad += 3 * (16 - len(d))
            if len(d) <= 8:
                pad += 1
            line += ' ' * pad

            for byte in d:
                # printable ASCII range 0x20 to 0x7E
                if 0x20 <= byte <= 0x7E:
                    line += chr(byte)
                else:
                    line += '.'
            retstr += line + '\n'
        return retstr

    # cs_disasm_quick 比 disasm 快一点
    # cs_disasm_lite 只返回(address, size, mnemonic, op_str) 4 个最重要的数据，比上面再快一点
    def disassemble_single(self, addr, size):
        """ Disassemble a single instruction at address """
        instruction = self.emu.mem_read(addr, size)
        return next(self.disasm.disasm_lite(bytes(instruction), addr, 1))

    def disassemble_single_detailed(self, addr, size):
        """ Disassemble a single instruction at addr """
        instruction = self.emu.mem_read(addr, 2 * size)
        return next(self.disasm.disasm(bytes(instruction), addr, 1))

    # show part
    def print_asmline(self,adr,mode,op_bytes, ins, op_str):
        """ Pretty-print assembly using pygments syntax highlighting """
        MODE = "T" if mode else "A"
        line = (
            highlight(f"{ins:<6} {op_str:<20}", self.asm_hl, self.asm_fmt)
                .decode()
                .strip("\n")
        )
        if len(op_bytes) == 4:
            op_bytes = f"{int(op_bytes, 16):04X}      "
        else:
            op_bytes = f"{int(op_bytes, 16):08X}  "


        print("\n" + MODE + self.color("YELLOW", f"{adr:8X}  ") + self.color("RED", op_bytes) + line, end=";")


    # 内存分配必须地址对齐
    def map_space(self, start, end, verbose=False):
        # 对start和end的地址做页对齐操作
        # 这儿使用位运算可以提高10%-20%的效率，但何必呢，总共也不会分配几次内存，咱就不牺牲那代码可读性了。

        # 这个memory 对齐的方式是不对的，在ends为整数时，会多map 4kb，这可不好。
        mem_start = self.round_down(start, self.page_size)
        mem_end = self.round_up(end, self.page_size)
        self.emu.mem_map(mem_start, mem_end - mem_start)
        # self.emu.mem_write(mem_start, bytes([1 for i in range(mem_end - mem_start)]))
        return 1

    # 向上对齐
    def round_up(self, value, size):
        if value % size == 0:
            return value
        return (value // size + 1) * size

    # 向下对齐
    def round_down(self, value, size):
        return (value // size) * size

    # write to memory
    def mem_write(self, address, data):
        self.emu.mem_write(address, data)

    # code trace
    def code_write(self, address, assembly, isa):
        isa = self._fix_isa(isa)


        pass

    # rainbow提供的两颗语法糖，非常优雅,可以看出，代码功底真的可。
    def __setitem__(self, inp, val):
        """ Sets a register, memory address or memory range to a value. Handles writing ints or bytes.

        Examples :

         - Write 0x1234 to register r0

          emulator['r0'] = 0x1234

         - Zero-out addresses 0x4000 to 0x4300

          emulator[0x4000:0x4300] = 0
         """
        ret = None
        if isinstance(inp, str):
            ret = self.emu.reg_write(self.reg_map[inp], val)
        elif isinstance(inp, int):
            if isinstance(val, int):
                v = val.to_bytes(4, "little")
                ret = self.emu.mem_write(inp, v)
            if isinstance(val, bytes):
                ret = self.emu.mem_write(inp, val)
        elif isinstance(inp, slice):
            if inp.start:
                start = inp.start
            else:
                start = 0

            if inp.step is not None:
                return NotImplementedError
            if isinstance(val, int):
                length = math.ceil(val.bit_length() / 8)
                v = val.to_bytes(length,"little")
                ret = self.emu.mem_write(start, v * (inp.stop - start))
            if isinstance(val,bytes):
                ret = self.emu.mem_write(start, val)
            if isinstance(val, bytearray):
                ret = self.emu.mem_write(start, bytes(val))
        else:
            raise Exception("Invalid range type for write : ", type(inp), inp)

    def __getitem__(self, s):
        """ Reads from a register using its shortname, or from a memory address/region. """
        if isinstance(s, str):  # regname
            return self.emu.reg_read(self.reg_map[s])
        # 第二种用法和意图，我了解不够深，想起来frida 游戏扫描脚本，那个或许可以用来扩展这儿。
        elif isinstance(s, int):
            return hex(int.from_bytes(self.emu.mem_read(s, 4), byteorder='little'))
        if isinstance(s, slice):
            # self[:1000]
            if s.start:
                start = s.start
            else:
                start = 0
            return self.emu.mem_read(start, s.stop - start)

    def color(self, name, x):
        return f"{getattr(colorama.Fore, name)}{x}{colorama.Fore.RESET}"

    # 一个重要的函数
    def d(self, content):
        address = self.HEAP_ADDR
        size = len(content)
        self.HEAP_ADDR += int(size // self.block + 1) * self.block
        self[content:content+size] = content
        return self.HEAP_ADDR

    # 断点调试时读取,TODO 增加utf8无法打印时的报错处理
    def readCString(self, address):
        buffer_address = address
        buffer_read_size = 0x10
        buffer = b""
        null_pos = None

        # Keep reading until we read something that contains a null terminator.
        while null_pos is None:
            buf_read = self[buffer_address:buffer_address + buffer_read_size]
            if b'\x00' in buf_read:
                null_pos = len(buffer) + buf_read.index(b'\x00')
            buffer += buf_read
            buffer_address += buffer_read_size
        return buffer[:null_pos].decode("utf-8")

    def readChar(self, address):
        return hex(int.from_bytes(self.emu.mem_read(address, 1), byteorder='little'))

    # 跳转回上层函数
    def return_force(self):
        self["pc"] = self["lr"]

    # patch code
    # def

    def inner_stubbed(self):
        # 内存分配
        @user_define
        def __aeabi_memset(emulator, addr, value, length):
            emulator[addr:addr + length] = value

        @user_define
        def __aeabi_memcpy(emulator, dist, source, size):
            print(hex(dist))
            print(hex(source))
            print(hex(size))
            emulator[dist:dist+size] = emulator[source:source+size]

        @user_define
        def __memcpy_chk(emulator, dist, source, size):
            emulator[dist:dist+size] = emulator[source:source+size]


        # 测试是否准确实现
        @user_define
        def memchr(emulator, address, char, size):
            for addr in range(address, address+size):
                if self.readChar(addr) == char:
                    return addr
            return 0

        @user_define
        def malloc(emulator,size):
            address = self.HEAP_ADDR
            self.HEAP_ADDR += int(size // self.block + 1) * self.block
            print(hex(address))
            return address

        @user_define
        def free(emulator,address):
            pass

        @user_define
        def strlen(emulator,address):
            content = self.readCString(address)
            length = len(content)
            print(f"content: {content}  length: {length}",end=" ")
            return length

        # 比较size大小的内存区块
        @user_define
        def memcmp(emulator, data1_ptr, data2_ptr, size):
            data1 = bytes(emulator[data1_ptr:data1_ptr + size])
            data2 = bytes(emulator[data2_ptr:data2_ptr + size])
            if operator.eq(data1, data2):
                return 0
            elif operator.gt(data1, data2):
                return 1
            else:
                return -1

        # 比较字符串，如果str2是str1的字串，str1》str2
        # 各种比较的返回值，只能确定相等时是0，大于小于是是啥，得看其具体实现，所以一般不使用其数值，只和0比较。
        @user_define
        def strcmp(emulator, str1_ptr, str2_ptr):
            str1 = self.readCString(str1_ptr)
            str2 = self.readCString(str2_ptr)
            if operator.eq(str1,str2):
                return 0
            elif operator.gt(str1,str2):
                return 1
            else:
                return -1

        @user_define
        def __android_log_print(emulator):
            pass

        @user_define
        def memclr(emulator):
            pass

        # 待验证
        @user_define
        def android_log_print(emulator, log_level, log_tag_ptr, log_format_ptr):
            params_count = len(locals())
            log_tag = self.readCString(log_tag_ptr)
            fmt = self.readCString(log_format_ptr)

            args_type = []
            args_count = 0
            i = 0
            while i < len(fmt):
                if fmt[i] == '%':
                    if fmt[i + 1] in ['s', 'd', 'p']:
                        args_type.append(fmt[i + 1])
                        args_count += 1
                        i += 1
                i += 1

            other_args = native_read_args(emulator, params_count - 2 + args_count)[
                         params_count - 2:]
            args = []
            for i in range(args_count):
                if args_type[i] == 's':
                    args.append(self.readCString(other_args[i]))
                elif args_type[i] == 'd' or args_type[i] == 'p':
                    args.append(other_args[i])

            # python not support %p format
            fmt = fmt.replace('%p', '0x%x')
            print("Called __android_log_print(%d, %s, %s)" % (
            log_level, log_tag, fmt % tuple(args)))


        # strncmp
        # https: // code.woboq.org / userspace / glibc / string / strncmp.c.html
        # 他娘的移植好累人，怪不得他们都直接加载模块，其实ctype库也能用。


        self.stubbed_functions["__aeabi_memset"] = __aeabi_memset
        self.stubbed_functions["memset"] = __aeabi_memset
        self.stubbed_functions["__aeabi_memcpy"] = __aeabi_memcpy
        self.stubbed_functions["memcpy"] = __aeabi_memcpy
        self.stubbed_functions["memchr"] = memchr
        self.stubbed_functions["malloc"] = malloc
        self.stubbed_functions["__memcpy_chk"] = __memcpy_chk
        self.stubbed_functions["free"] = free
        self.stubbed_functions["strlen"] = strlen
        self.stubbed_functions["__strlen_chk"] = strlen
        self.stubbed_functions["memcmp"] = memcmp
        self.stubbed_functions["strcmp"] = strcmp
        self.stubbed_functions["__android_log_print"] = __android_log_print
        self.stubbed_functions["__aeabi_memclr4"] = memclr
        self.stubbed_functions["__aeabi_memclr8"] = memclr


    def inner_jni(self):
        base = 0x1000
        self[self.JNI_ADDR] = self.JNI_ADDR + base
        for index in range(4,300):
            self[self.JNI_ADDR + base + index*4] = self.JNI_ADDR_FUC + index*4 + 1
            self.jni_functions[self.JNI_ADDR_FUC + index*4] = index

        with open(r"C:\Users\pr0214\Desktop\DTA\SO进阶课\PycharmProjects\uTracer\jni_env.txt", encoding="utf-8",mode="r")as F:
            jni_funcs = F.readlines()
            jni_funcs = [i.strip().replace(",","")  for i in jni_funcs]
            self.jni_functions_names = jni_funcs


# 关于装饰器 https://github.com/AeonLucid/AndroidNativeEmu/commit/5724bd97c4c2722ea1101e128bc5eef7d1d69850
# 装饰器是一个面向用户的东西，它到底让模拟执行变的优雅了，还是变得晦涩和不清晰了呢？后续我们可能会去掉它。

def native_read_args(mu, args_count):
    native_args = []

    if args_count >= 1:
        native_args.append(mu["r0"])

    if args_count >= 2:
        native_args.append(mu["r1"])

    if args_count >= 3:
        native_args.append(mu["r2"])

    if args_count >= 4:
        native_args.append(mu["r3"])

    sp = mu["sp"] + 8

    if args_count >= 5:
        for x in range(0, args_count - 4):
            native_args.append(int.from_bytes(mu.mem_read(sp + (x * 4), 4), byteorder='little'))

    return native_args


def user_define(func):
    def native_method_wrapper(*argv):
        """
        :type self
        :type emu androidemu.emulator.Emulator
        :type mu Uc
        """

        emu = argv[0]

        args = inspect.getfullargspec(func).args
        args_count = len(args) - 1

        if args_count < 0:
            raise RuntimeError(
                "NativeMethod accept at least (self, mu) or (mu).")

        native_args = native_read_args(emu, args_count)
        result = func(emu, *native_args)

        ## 3.30 测试一下这部分OK不
        if result:
            if isinstance(result, int):
                emu["r0"] = result
            else:
                raise NotImplementedError("Unable to write response '%s' to emulator." % str(result))

    return native_method_wrapper

# TODO HOOK实现import表的函数或者so中某些函数

# 1已经解决；2通过回避的方式解决。frida 之所以readCString，readPointer，是因为人家命令多，而且是编程方式。
# 调试时，应该命令少于三个英文字母，又贴合意思比较好！
# TODO hexdump 有两个地方需要修复，一是hexdump时的index最好和address保持一致，即向frida靠齐(这点必须得实现，不然整体功能和效果影响很大)，二是输入，似乎pycharm有bug,(1这样的输出有问题，导致没法“hexdump(100,10),只能hexdump，我觉得这样不是很好。”

# TODO HOOK方式实现一些系统调用，优先级不太高，因为我还没遇过 测试试试
# TODO 支持函数的方式调用
# TODO 实现京东HOOK并得出结果，这就证明Utrace可以作为ida trace 的替代
# TODO 能否在OLLVM上起作用呢？这是非常关键和重要的，如果我搞出来，我就是牛逼plus。
# TODO 发现一个大问题，在工程实践中果然最能发现问题：你可以不实现JNI函数，但无论如何你得“识别”JNI函数。他妈的，光识别还不够。
# TODO 既然主打的不是模拟执行，那它必须有unidbg/androidnativeemu无法替代的地方，有几个方向可以试图弄一下：ollvm反混淆、字符串解密、JNUPout修复。

# TODO 提供一种和block hook 不同的 ，打patch的方式。根据代码是arm 还是thumb 打patch。我有个忧虑，我怀疑它只能在thumb上运行，arm上可能会出问题。
# TODO 处理init
# TODO 在LINUX上使用C++ fit ，windows上使用demangle √
# TODO 可以选择展示opcode以及thumb/arm √
# TODO plasma 支持区分是sub还是loc还是byte欸！这很好！ 我做不到 ×
# TODO JNI真他妈堵人，真他妈堵人 ，真他妈堵人
# TODO 使用更高级，更好看，更有用的hexdump √
# TODO 对于一些糟糕的敏感的东西，进行嗅探，比如xor指令，比如这部分可能是arm加密。
# 待验证
# TODO 增加内存块的监控 参考unicorn_tracer，其实arm trace中已经有了这些，但我们希望以一种更直观的方式实现这一种很重要的功能。通过printf函数来测试
# TODO OLLVM 反混淆
# TODO trace做的非常好的工具应该早就有了，因为本身实现并不复杂，得结合自身特长做出亮点！——我的特长可能是密码学？？
# TODO 通过修改或者replace替换函数实现skip_functions功能
# TODO 实现交互式的调试器，实现步过
# TODO 应该有办法识别程序的结束位置吧？？
#                                 ---=== WRITE ===---
# 000000  0d                                                     |.               |
#
#                                 ---=== READ ===---
# 000000  0d 0a 4c 6f 67 69 6e 3a  20                            |..Login:        |
#
#                                 ---=== WRITE ===---
# 000000  61 64 6d 69 6e 0d                                      |admin.          |
#
#                                 ---=== READ ===---
# 000000  0d 0a 50 61 73 73 77 6f  72 64 3a 20                   |..Password:     |
# 如果想实现对读写的监控，需要对hook memory read/write进行一些封装和逻辑判断。这是很好的。
# 如果单纯想“写”，也可以hook内存块，每次看和上次是否有变化，也是可以的。
# 突然意识到，rainbow的函数hook方式，效率可能会比较低?因为是用block hook实现的，每到一个函数就要停一次，依赖的是unicorn本身的hook机制，而androidnativeemu
# 更像got表 hook，可以只在运行到自己开辟的那一片用于替换函数的特殊内存才hook和回调。
# TODO目标
# 把androidnativeemu的例子都跑一边（难度很不小）

# c++ name d
def name_filt(name):
    try:
        env = platform.system()
        if env == "Windows":
            process_results = subprocess.run(r"C:\Users\Lenovo\PycharmProjects\uTracer\tools\cpp_demangle {}".format(name),
                                             stdout=subprocess.PIPE,
                                             stderr=subprocess.PIPE)
            return process_results.stdout.decode("utf-8").strip()
        else:
            import cxxfilt
            return cxxfilt.demangle(name)
    except:
        return name


