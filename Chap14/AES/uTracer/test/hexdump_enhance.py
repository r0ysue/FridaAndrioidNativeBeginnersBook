import sys
import hexdump

import uTracer


def advance_dump(data, base):
    generator = hexdump.genchunks(data, 16)
    retstr = ''
    for addr, d in enumerate(generator):
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


# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/base_md5.so")
import hexdump
hexdump.hexdump(emulator[:1000])
print(advance_dump(emulator[:1000],0x45F))
# OK
print(sys.version_info)