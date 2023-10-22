import binascii
import uTracer

"""
JNI 测试
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/testjni.so")

# execute range
target = 0xB58 | 1
target_end = 0xBA6 | 1


# 参数准备以及填充，这里还很缺封装
base = emulator.DATA_ADDR
env = emulator.JNI_ADDR
plain_text = b'123456'
in_addr = base + 0x100
emulator[in_addr] = plain_text
emulator["r0"] = env
emulator["r1"] = 0
emulator["r2"] = in_addr

# emulator[0xB500000] = 12
# emulator.hexdump(0xB500000,100)


# 运行
emulator.start(target,target_end)

# 打印输出结果
out_buf = emulator["r0"]
result = emulator[out_buf:out_buf+16]
print(binascii.b2a_hex(result))

