import binascii
import uTracer

"""
最右SIGN
经过轻度OLLVM的样本
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/right.so")

# execute range
target = 0x4A28C + 1
target_end = 0x4A372

#
base = emulator.DATA_ADDR
plain_text = b'123456'
in_addr = base + 0x100
out_buffer = base + 0x200
emulator[in_addr] = plain_text
emulator[out_buffer] = b"\x00" * 16
emulator["r0"] = in_addr
emulator["r1"] = len(plain_text)
emulator["r2"] = out_buffer


# 运行
emulator.start(target,target_end)

# 打印输出结果
out_buf = emulator["r0"]
result = emulator[out_buf:out_buf+16]
print(binascii.b2a_hex(result))

