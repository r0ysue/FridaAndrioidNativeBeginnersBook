'''
sha0,sha1,sha256,sha512
sha0 的支持在openssl 1.1.0中删掉了，这很合理，sha1和sha0只有一个微小差异，但sha0更不安全，留他干嘛。
    for i in range(16, 80):
        w[i] = _left_rotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1) #sha1
        w[i] = (w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]) #sha0
'''

import binascii
import uTracer

"""
OPENSSL 标准MD5 实现
无需任何额外操作
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load(r"C:\Users\Lenovo\PycharmProjects\uTracer\example\lib\libcrypto.so")

# execute range
target = 0x83914 | 1
target_end = 0x83966

# 参数准备以及填充，这里还很缺封装
base = emulator.DATA_ADDR
plain_text = b'123456789'
in_addr = base + 0x100
out_buffer = base + 0x200
emulator[in_addr] = plain_text
emulator[out_buffer] = b"\x00" * 20
emulator["r0"] = in_addr
emulator["r1"] = len(plain_text)
emulator["r2"] = out_buffer


# 运行
emulator.start(target,target_end)

# 打印输出结果
result = emulator[out_buffer:out_buffer+20]
print(binascii.b2a_hex(result))



