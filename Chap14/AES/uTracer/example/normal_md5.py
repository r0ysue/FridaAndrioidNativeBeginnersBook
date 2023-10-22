import binascii
import uTracer
from uTracer import user_define

"""
不适用查表法实现的MD5，导入了math库
无需任何额外操作
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/normal_md5.so")

# execute range
target = 0xd08 + 1
target_end = 0x1004

# 参数准备以及填充，这里还很缺封装
base = emulator.DATA_ADDR
plain_text = b'123456'
in_addr = base + 0x100
emulator[in_addr] = plain_text
emulator["r0"] = in_addr
emulator["r1"] = len(plain_text)

# HOOK
# calcKs 函数生成K表，入参是一个指针，K表就放在这个地址开头的内存上。实现这个函数。
@user_define
def calcKs(emulator, k0):
    # 写入K表
    k_table = "78a46ad756b7c7e8db702024eecebdc1af0f7cf52ac68747134630a8019546fdd8988069aff7448bb15bffffbed75c892211906b937198fd8e4379a62108b44962251ef640b340c0515a5e26aac7b6e95d102fd65314440281e6a1d8c8fbd3e7e6cde121d60737c3870dd5f4ed145a4505e9e3a9f8a3effcd9026f678a4c2a8d4239faff81f6718722619d6d0c38e5fd44eabea4a9cfde4b604bbbf670bcbfbec67e9b28fa27a1ea8530efd4051d880439d0d4d9e599dbe6f87ca21f6556acc4442229f497ff2a43a72394ab39a093fcc3595b6592cc0c8f7df4efffd15d84854f7ea86fe0e62cfe144301a3a111084e827e53f735f23abdbbd2d72a91d386eb"
    emulator[k0] = bytes.fromhex(k_table)


emulator.stubbed_functions["_Z6calcKsPj"] = calcKs
# 运行
emulator.start(target,target_end)

# 打印输出结果
out_buf = emulator["r0"]
result = emulator[out_buf:out_buf+16]
print(binascii.b2a_hex(result))

