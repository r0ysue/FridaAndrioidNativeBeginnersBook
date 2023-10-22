import binascii
import uTracer

"""
OPENSSL 标准MD5 实现
无需任何额外操作
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/des.so")

# execute range
target = 0x99C8 + 1
target_end = 0x9A36


# 设置方法1
# emulator["r0"] = in_addr
# emulator["r1"] = len(plain_text)
# emulator["r2"] = out_buffer


# 运行
emulator.start(target,target_end)
# emulator.start

# 打印输出结果
out_buf = emulator["r2"]
result = emulator[out_buf:out_buf+16]
print(binascii.b2a_hex(result))

# emulator.dump_reg()
# print(emulator.r0)
# print(emulator.r1)
# print(emulator.r2)
# print(emulator.r3)

