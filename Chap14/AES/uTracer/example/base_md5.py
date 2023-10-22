import binascii
import uTracer

"""
OPENSSL 标准MD5 实现
无需任何额外操作
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/base_md5.so")

# execute range
target = 0x21F0 + 1
target_end = 0x226C


# 参数准备以及填充，这里还很缺封装
base = emulator.DATA_ADDR
plain_text = b'123456'
in_addr = base + 0x100
out_buffer = base + 0x200

emulator[in_addr] = plain_text
emulator[out_buffer] = b"\x00" * 16
# 设置方法1
emulator["r0"] = in_addr
emulator["r1"] = len(plain_text)
emulator["r2"] = out_buffer

# # 设置方法2
# emulator.set_reg(r0=in_addr, r1=len(plain_text), r2=out_buffer)

# 运行
emulator.start(target,target_end)
# emulator.start

# 打印输出结果
out_buf = emulator["r0"]
result = emulator[out_buf:out_buf+16]
print(binascii.b2a_hex(result))



