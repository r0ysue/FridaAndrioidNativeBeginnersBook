# 导入
import uTracer

# 初始化模拟器
emulator = uTracer.uTrace()
# 加载SO
emulator.load(r"C:\Users\pr0214\Desktop\DTA\SO进阶课\PycharmProjects\uTracer\example\lib\libcrack.so")

# 起始地址和终止地址
target = 0x7D60 + 1
target_end = 0x7D9E

# 参数准备以及填充，这里还很缺封装
base = emulator.DATA_ADDR

# 参数1
# 明文
message = b"0123456789abcdef"
message_addr = base + 0x50
emulator[message_addr] = message

# 参数2
# 密钥
key = b"23456789abcdef10"
key_addr = base + 0x100
emulator[key_addr] = key

# 参数3
out_buf = base + 0x200

# 传入参数
emulator["r0"] = message_addr
emulator["r1"] = key_addr
emulator["r2"] = out_buf

emulator.start(target, target_end)

# result = emulator[out_buf:out_buf+16]
# print(binascii.b2a_hex(result))

emulator.hexdump(out_buf, 0x10)