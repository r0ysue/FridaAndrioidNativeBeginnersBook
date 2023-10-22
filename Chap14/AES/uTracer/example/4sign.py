import binascii
import uTracer

"""
测试4个sign
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/4sign.so")

base = emulator.DATA_ADDR


# TEA算法
def sign1():
    emulator.bkpt_dump()
    # execute range
    target = 0x9B68 + 1
    target_end = 0x9C2C

    in_addr = base + 0x100
    emulator[in_addr] = b'123'
    emulator["r0"] = in_addr

    emulator.start(target,target_end)

    out_buf = emulator["r2"]
    result = emulator[out_buf:out_buf+16]
    print(binascii.b2a_hex(result))
    emulator.reset()


# MD5 算法
def sign2():
    emulator.bkpt_dump()
    # execute range
    target = 0x9C6C + 1
    target_end = 0x9CAE

    in_addr = base + 0x100
    out_buffer = base + 0x200
    emulator[in_addr] = b'123456'
    emulator[out_buffer] = b"\x00" * 16
    emulator["r0"] = in_addr
    emulator["r1"] = out_buffer

    emulator.start(target,target_end)

    out_buf = emulator["r1"]
    result = emulator[out_buf:out_buf+16]
    print(binascii.b2a_hex(result))
    emulator.reset()

# sign1()
# 需要read|map 的hook
# 依然有bug ，原因未知。
sign2()
