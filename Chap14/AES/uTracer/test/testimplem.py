import binascii
import uTracer
import hexdump
import operator
"""
测试4个sign
"""

# 初始化模拟器
emulator = uTracer.uTrace()
# load so file
emulator.load("./lib/4sign.so")

emulator[0x10:0x14] = b"\x7F\x45\x4C\x46"
hexdump.hexdump(emulator[:0x100])

# int
# memcmp (const PTR str1, const PTR str2, size_t count)
# {
#   register const unsigned char *s1 = (const unsigned char*)str1;
#   register const unsigned char *s2 = (const unsigned char*)str2;
#   while (count-- > 0)
#     {
#       if (*s1++ != *s2++)
#           return s1[-1] < s2[-1] ? -1 : 1;
#     }
#   return 0;
# }


def memcmp(data1_ptr, data2_ptr, size):
    data1 = bytes(emulator[data1_ptr:data1_ptr + size])
    data2 = bytes(emulator[data2_ptr:data2_ptr + size])
    print(operator.gt(data1,data2))
    if operator.eq(data1,data2):
        return 0
    elif operator.gt(data1,data2):
        return 1
    else:
        return -1

# ret = memcmp(0x10,0x0,4)
# print(ret)


def strcmp(data1_ptr, data2_ptr):
    data1 = emulator.readCString(data1_ptr)
    data2 = emulator.readCString(data2_ptr)
    print(operator.gt(data1,data2))
    if operator.eq(data1,data2):
        return 0
    elif operator.gt(data1,data2):
        return 1
    else:
        return -1

ret = strcmp(0x0,0x10)
print(ret)