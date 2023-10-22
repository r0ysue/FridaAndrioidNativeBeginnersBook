import binascii
import uTracer

"""
OPENSSL 标准MD5 实现
无需任何额外操作
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/openssl_aes.so")

# execute range
target = 0x8E80 + 1
target_end = 0x8FBC

base = emulator.DATA_ADDR

key = base + 0x200
iv = base + 0x300
in_buf = base + 0x100
out_buf = base + 0x400


emulator[in_buf] = b"lilac"
emulator[key] = b"wandoer.com_wandoer.com_wandoer."
emulator[iv] = b"_iv_must_be_16B_"
emulator["r0"] = key
emulator["r1"] = iv
emulator["r2"] = in_buf
emulator["r3"] = out_buf
# sp = emulator["sp"] + 8
# len_text = 5
# emulator.emu.mem_write(sp, len_text.to_bytes(4, "little"))
# if args_count >= 5:
#     for x in range(0, args_count - 4):
#         native_args.append(
#             int.from_bytes(mu.mem_read(sp + (x * 4), 4), byteorder='little'))

# 运行
emulator.start(target,target_end)

# 打印输出结果
out_buf = emulator["r0"]
print(out_buf)
emulator.hexdump(out_buf, 0x10)
# # print(out_buf)
# result = emulator[out_buf:out_buf+10]
# print(binascii.b2a_hex(result))

