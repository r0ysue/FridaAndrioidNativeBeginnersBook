from aes import AES

master_key = 0x2b7e151628aed2a6abf7158809cf4f3c
# master_key = 0x7469656d306e61756b6e617331696175
# 密钥的编排
# w0-w3 是 本身的 key
# w0 [2b,7e,15,16]
# w1 [28,ae,d2,a6]
# w2 [ab,f7,15,88]
# w3 [09,cf,4f,3c]
# w4 到 w43 这 10个密钥 是生成的。

AES = AES(master_key)
plaintext = 0x483e3f3d3f3c3f3b3f3a3f393f383f37
encrypted = AES.encrypt(plaintext)
print(hex(encrypted))

# AES 或者 DES 又或者别的分组加密算法的demo，教学
# 都是不包括分组模式、padding这些内容的，因为这样简单一些，或者就是padding不正确。
