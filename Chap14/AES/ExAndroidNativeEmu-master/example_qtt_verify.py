import base64
import logging
import posixpath
import sys
import time
import uuid

from hexdump import hexdump
from unicorn import *
from unicorn.arm_const import *

from androidemu.emulator import Emulator
import androidemu.utils.debug_utils
from androidemu.java.java_class_def import JavaClassDef
from androidemu.java.java_method_def import java_method_def
from androidemu.utils.chain_log import ChainLogger
from androidemu.java.classes.string import String
from androidemu.java.classes.list import List
from androidemu.java.classes.array import Array
from androidemu.utils.memory_helpers import read_byte_array


class InnoSecureUtils(metaclass=JavaClassDef, jvm_name='com/inno/innosecure/InnoSecureUtils'):
    def __init__(self):
        pass

    @java_method_def(name='secure', signature='(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[B)[B',
                     native=True)
    def secure(self, mu):
        pass



def hook_mem_read(uc, access, address, size, value, user_data):
    """
    (    libInnoSecure.so[0xCBBCB000])[20 22       ]0x00006F4C:	MOVS	R2, #0X20
    (    libInnoSecure.so[0xCBBCB000])[FA F7 7C ED ]0x00006F4E:	BLX	#0XCBBCCA48	;(PC=0xCBBD1F4E )
    """
    if (address == int("0x00006F4E", 16)):
        print("read1411")
        # data = uc.mem_read(address, size)
        # v = int.from_bytes(data, byteorder='little', signed=False)
        # print("read1111")
        # print(v)


def jb2pb(byte_arr):
    """
    java 字节数组转python字节数组
    :return:
    """

    return [i + 256 if i < 0 else i for i in byte_arr]


def hook_mem(uc, access, address, size, value, user_data):
    pass


g_cfd = ChainLogger(sys.stdout, "./ins-qtt-3liu-verify.txt")

# Add debugging.
def hook_code(mu, address, size, user_data):
    try:
        emu = user_data
        if (not emu.memory.check_addr(address, UC_PROT_EXEC)):
            logger.error("addr 0x%08X out of range" % (address,))
            sys.exit(-1)
        # 第一个分组的原文和IV
        if hex(address - base_address).upper() == "0X6CEA":
            r1 = mu.reg_read(UC_ARM_REG_R1)
            r2 = mu.reg_read(UC_ARM_REG_R2)
            print("plaintext")
            print(hex(r1))
            print(hex(r2))
        # 原文 0123456789123456
        # 原文hex 30313233343536373839313233343536
        # IV：b8b16602faaf76d8c50afacc31dd882f


        # CBC 工作模式 xor with iv
        if hex(address - base_address).upper() == "0X6CF4":
            r5 = mu.reg_read(UC_ARM_REG_R5)
            print("After xor with iv :")
            hexdump(read_byte_array(mu, r5, 16))
        # 88 80 54 31 ce 9a 40 ef fd 33 cb fe 02 e9 bd 19
        # 已验证
        # https://gchq.github.io/CyberChef/#recipe=From_Hex('Auto')XOR(%7B'option':'Hex','string':'b8b16602faaf76d8c50afacc31dd882f'%7D,'Standard',false)To_Hex('Space',0)&input=MzAzMTMyMzMzNDM1MzYzNzM4MzkzMTMyMzMzNDM1MzY

        # addroundkeys 1
        if hex(address - base_address).upper() == "0X6D06":
            print("frist addroundkeys")
            r2 = mu.reg_read(UC_ARM_REG_R2)
            print(hex(r2))
            # KEY
            # fd22c75652058294ad0d241988aef851

        # after addroundkeys 1
        if hex(address - base_address).upper() == "0X6D76":
            print("After first add roundkeys")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            print(hex(r1))
        # 75a293679c9fc27b503eefe78a474548

        # 查S表
        # 手算应该是9d.....52
        if hex(address - base_address).upper() == "0X6D80":
            print("After first sbox replace")
            r5 = mu.reg_read(UC_ARM_REG_R5)
            hexdump(read_byte_array(mu, r5, 16))
        # 9D 3A DC 85 DE DB 25 21  53 B2 DF 94 7E A0 6E 52

        # 循环左移
        # 循环左移后
        if hex(address - base_address).upper() == "0X6D28":
            print("After first 循环左移")
            r0 = mu.reg_read(UC_ARM_REG_R0)
            hexdump(read_byte_array(mu, r0, 16))
            # print(hex(r0)) #0x100ffad8
        # 9D DB DF 52 DE B2 6E 85  53 A0 DC 21 7E 3A 25 94

        # mixcolumn
        # clion： da 18 15 1c   81 96 24 b4   a0 56 33 cb   03 f1 a9 ae
        if hex(address - base_address).upper() == "0X6D2C":
            print("After first mixcolumn")
            r5 = mu.reg_read(UC_ARM_REG_R5)
            hexdump(read_byte_array(mu, r5, 16))
            # r5 的方式总是有些无法解释和不保险，也可以通过地址来
            # DA 18 15 1C 81 96 24 B4  A0 56 33 CB 03 F1 A9 AE
            # 即得到进去时的上面的地址
            print("查看mixcolumn后的地址")
            hexdump(read_byte_array(mu,0x100ffad8,16))
            # DA 18 15 1C 81 96 24 B4  A0 56 33 CB 03 F1 A9 AE

        # subbytes+循环左移+mixcolumn都是标准的，接下来看最难的addroundkey

        if hex(address - base_address).upper() == "0X6E18":
            print("final result !!!")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            hexdump(read_byte_array(mu, r1, 16))

        # final result !!!
        # 00000000: D6 3B 7A 75 F1 D0 DF 75  97 C2 C7 9A D7 50 47 BA  .;zu...u.....PG.
        # final result !!!
        # 00000000: 77 B1 FF 18 80 EC 94 3A  83 E2 F1 09 6C 95 21 63  w......:....l.!c
        # d63b7a75f1d0df7597c2c79ad75047ba
        # 77b1ff1880ec943a83e2f1096c952163


    except Exception as e:
        logger.exception("exception in hook_code")
        sys.exit(-1)
    #


#


logger = logging.getLogger(__name__)

# Initialize emulator
emulator = Emulator(
    vfp_inst_set=True,
    vfs_root=posixpath.join(posixpath.dirname(__file__), "vfs")
)

lib_module = emulator.load_library("tests/bin/libInnoSecure.so")

emulator.java_classloader.add_class(InnoSecureUtils)

# Show loaded modules.
# logger.info("Loaded modules:")
for module in emulator.modules:
    if "libInnoSecure" in module.filename:
        base_address = module.base
        base_size = module.size
        logger.info("base_address=> 0x%08x - %s" % (module.base, module.filename))

        break

emulator.mu.hook_add(UC_HOOK_CODE, hook_code, emulator)
emulator.mu.hook_add(UC_HOOK_MEM_READ|UC_HOOK_MEM_WRITE, hook_mem, emulator)

try:
    emulator.call_symbol(lib_module, 'JNI_OnLoad', emulator.java_vm.address_ptr, 0x00)

    st = time.time()
    a1 = "0123456789123456"
    a2 = "af54ad64-e217-4667-a6b2-fa503a156901"
    a3 = "3082021b30820184a0030201020204574beab6300d06092a864886f70d01010505003052310c300a06035504061303303231310b3009060355040813025348310b3009060355040713025348310b3009060355040a13025a48310b3009060355040b1302434e310e300c0603550403130551754b616e301e170d3136303533303037323433385a170d3431303532343037323433385a3052310c300a06035504061303303231310b3009060355040813025348310b3009060355040713025348310b3009060355040a13025a48310b3009060355040b1302434e310e300c0603550403130551754b616e30819f300d06092a864886f70d010101050003818d0030818902818100aa5bae49b771380e692444437b82b375cabdefb3f23307c29510653776b8e4115f776bea5eb6690285f97d4e6e8d0469e49f79ecba31e4b7fb85dd612ee6b27ef38502aa38d055ddad2aa7b52d19fb8d2aeeb59a830b91c341f1b467655e7313e9ff65feb6539bf1655f35a37e17faa94e506a08219df196730f45d9c1cd94d30203010001300d06092a864886f70d0101050500038181000e6cc9fb74aef11dd33d6603869a9db61b8dcedae77bc815433026693fe59fd4b75a3284170f8872737e55595c1fd40da3dfbe5ad8a4e96802f53637977f0eb6e9b0dc35161cbaed398b41ecd73c4009a1dae7bcb00b75c3f8d5792405bcc5e4602d9dff6a0dc4739240a3b42626f5efce4d7baea0fced2b13361cb4ded8ed0b"
    a4_baar = [-119, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 10, 0, 0, 0, 10, 8, 2, 0, 0,
               0, 2, 80,
               88, -22, 0, 0, 1, 110, 73, 68, 65, 84, 120, 1, 0, 31, 0, -32, -1, 0, 24, 118, 35, 107, -86, 3, -77,
               26, 72, 5,
               -5, -118, 41, 42, 69, -6, 56, 45, 91, 91, -57, 47, 81, 16, -40, 113, -24, -41, -1, -29, 0, 31, 0,
               -32, -1, 0,
               -20, 67, 96, 29, 74, 113, 44, 9, 43, 80, -60, 97, -116, 7, 67, -72, -84, -39, -50, -50, 0, 29, 54,
               86, -105,
               122, 36, 9, 60, -90, 0, 31, 0, -32, -1, 0, 9, 89, -123, 99, -52, 105, -94, 40, -30, 43, 123, 112,
               -37, 72, -54,
               87, -52, 61, -88, -55, -74, 90, -57, 66, -117, -69, 66, -58, -115, 94, 0, 31, 0, -32, -1, 0, 121, 56,
               4, 97,
               -28, 85, 11, 108, -11, -50, -50, 0, 125, 31, -126, -34, 29, -62, -120, 117, -118, -39, 76, -60, 36,
               -9, 35,
               115, -34, 44, 0, 31, 0, -32, -1, 0, -40, -35, 125, 15, 55, 70, 105, 69, 69, 117, -46, 45, 109, -17,
               87, -113,
               -29, 77, 34, 104, 74, -73, -120, 26, 120, -96, 118, -97, -124, 16, 0, 31, 0, -32, -1, 0, 91, -104,
               -117, 101,
               25, -108, -85, 74, 63, -10, -77, 9, 24, 117, 60, -46, 0, -77, -55, -53, -81, -120, -102, 105, 51,
               -119, 61,
               -127, 21, -45, 0, 31, 0, -32, -1, 0, -51, 26, 122, -117, -48, 6, 8, 98, -26, -19, -63, 95, 113, -31,
               60, -61,
               -42, 22, -48, 35, -18, -118, -60, 29, 111, -6, 64, -7, 104, 100, 0, 31, 0, -32, -1, 0, -12, 103, 77,
               123, -125,
               44, -92, 28, 125, 6, 118, -35, -74, 102, -21, -72, -119, 100, 92, 84, 43, 48, -13, -51, -59, -93,
               -37, 93,
               -114, 40, 0, 31, 0, -32, -1, 0, -53, 28, -7, 57, -30, -74, -47, -42, -33, 44, 99, -35, 73, 62, -64,
               -4, 90,
               -88, -121, 127, -2, -32, -63, 113, -24, -23, -84, -69, 46, -88, 1, 31, 0, -32, -1, 0, -20, 62, 122,
               -41, 80,
               19, 51, -31, 5, -44, 16, -7, 25, 41, -59, -79, -59, -11, 105, -126, -34, 115, -87, -108, 113, 118,
               -74, 76, 37,
               27, -64, -45, -109, 113, -14, 52, 7, 91, 0, 0, 0, 0, 73, 69, 78, 68, -82, 66, 96, -126]
    a4 = Array(bytearray(jb2pb(a4_baar)))
    result = InnoSecureUtils().secure(emulator, String(a1), String(a2), String(a3), a4)
    b64_res = base64.b64encode(bytes(result)).decode()
    # print(result)
    print(b64_res)
    et = time.time()
    print(f"{(et - st)}秒")
except UcError as e:
    print("Exit at %x" % emulator.mu.reg_read(UC_ARM_REG_PC))
    raise
