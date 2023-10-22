import binascii

import unicorn

import uTracer

"""
OPENSSL 标准MD5 实现
无需任何额外操作
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load(r"C:\Users\pr0214\Desktop\DTA\SO进阶课\PycharmProjects\uTracer\example\lib\libInnoSecure.so")

# execute range
target = 0x6154 + 1
target_end = 0x6154 + 0xE8c


# 参数准备以及填充，这里还很缺封装
base = emulator.DATA_ADDR

message = b"12345678"
uuid = b"af54ad64-e217-4667-a6b2-fa503a156901"
sign2 = b"3082021b30820184a0030201020204574beab6300d06092a864886f70d01010505003052310c300a06035504061303303231310b3009060355040813025348310b3009060355040713025348310b3009060355040a13025a48310b3009060355040b1302434e310e300c0603550403130551754b616e301e170d3136303533303037323433385a170d3431303532343037323433385a3052310c300a06035504061303303231310b3009060355040813025348310b3009060355040713025348310b3009060355040a13025a48310b3009060355040b1302434e310e300c0603550403130551754b616e30819f300d06092a864886f70d010101050003818d0030818902818100aa5bae49b771380e692444437b82b375cabdefb3f23307c29510653776b8e4115f776bea5eb6690285f97d4e6e8d0469e49f79ecba31e4b7fb85dd612ee6b27ef38502aa38d055ddad2aa7b52d19fb8d2aeeb59a830b91c341f1b467655e7313e9ff65feb6539bf1655f35a37e17faa94e506a08219df196730f45d9c1cd94d30203010001300d06092a864886f70d0101050500038181000e6cc9fb74aef11dd33d6603869a9db61b8dcedae77bc815433026693fe59fd4b75a3284170f8872737e55595c1fd40da3dfbe5ad8a4e96802f53637977f0eb6e9b0dc35161cbaed398b41ecd73c4009a1dae7bcb00b75c3f8d5792405bcc5e4602d9dff6a0dc4739240a3b42626f5efce4d7baea0fced2b13361cb4ded8ed0b"
message_addr = base + 0x50
emulator[message_addr] = message
uuid_addr = base + 0x100
emulator[uuid_addr] = uuid
sign2_addr = base + 0x200
emulator[sign2_addr] = sign2
sign2_length = len(sign2)
result_addr = base + 0x1500
emulator[result_addr] = b""
result_length = 0
sign3 = b""
sign3_address = base + 0x1600
emulator[sign3_address] = sign3

# emulator[message_addr] = message
# emulator[out_buffer] = b"\x00" * 16
# 设置方法1
emulator["r0"] = message_addr
emulator["r1"] = uuid_addr
emulator["r2"] = sign2_addr
emulator["r3"] = sign2_length

emulator.emu.mem_write(0xe00000 - 4, result_addr.to_bytes(4, byteorder='little'))
emulator.emu.mem_write(0xe00000 - 8, result_length.to_bytes(4, byteorder='little'))
emulator.emu.mem_write(0xe00000 - 12, sign3_address.to_bytes(4, byteorder='little'))

emulator.start(target,target_end)