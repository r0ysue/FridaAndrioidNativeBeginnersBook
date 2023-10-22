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


g_cfd = ChainLogger(sys.stdout, "./ins-qtt-3liu-verify-16.txt")

# Add debugging.
def hook_code(mu, address, size, user_data):
    try:
        emu = user_data
        if (not emu.memory.check_addr(address, UC_PROT_EXEC)):
            logger.error("addr 0x%08X out of range" % (address,))
            sys.exit(-1)
        #
        # if address in range(base_address, base_address+base_size):
        #     androidemu.utils.debug_utils.dump_code(emu, address, size, g_cfd)

        # 最终结果
        # RDhDNzlFRTUxQ0REMDA3RDYwMjk0MTQ1OEY4M0I1RDQuY0dGeVlXMGZZV1kxTkdGa05qUXRaVEl4TnkwME5qWTNMV0UyWWpJdFptRTFNRE5oTVRVMk9UQXhIblpsY25OcGIyNGZNVEVlY0d4aGRHWnZjbTBmWVc1a2NtOXBaQjVsWXg4eC4olmS1oPmsw9QU1BUsJNBXH5mtZExLZGNtu / NrFr9kZhHzNg ==
        # base 64前
        # 00000000  44 38 43 37 39 45 45 35 31 43 44 44 30 30 37 44  |D8C79EE51CDD007D|
        # 00000010  36 30 32 39 34 31 34 35 38 46 38 33 42 35 44 34  |602941458F83B5D4|
        # 00000020  2e 63 47 46 79 59 57 30 66 59 57 59 31 4e 47 46  |.cGFyYW0fYWY1NGF|
        # 00000030  6b 4e 6a 51 74 5a 54 49 78 4e 79 30 30 4e 6a 59  |kNjQtZTIxNy00NjY|
        # 00000040  33 4c 57 45 32 59 6a 49 74 5a 6d 45 31 4d 44 4e  |3LWE2YjItZmE1MDN|
        # 00000050  68 4d 54 55 32 4f 54 41 78 48 6e 5a 6c 63 6e 4e  |hMTU2OTAxHnZlcnN|
        # 00000060  70 62 32 34 66 4d 54 45 65 63 47 78 68 64 47 5a  |pb24fMTEecGxhdGZ|
        # 00000070  76 63 6d 30 66 59 57 35 6b 63 6d 39 70 5a 42 35  |vcm0fYW5kcm9pZB5|
        # 00000080  6c 59 78 38 78 2e 28 96 64 b5 a0 f9 ac c3 d4 14  |lYx8x.(.dµ ù¬ÃÔ.|
        # 00000090  d4 15 2c 24 d0 57 1f 99 ad 64 4c 4b 64 63 6d bb  |Ô.,$ÐW...dLKdcm»|
        # 000000a0  f3 6b 16 bf 64 66 11 f3 36                       |ók.¿df.ó6|

        # 输出结果
        # if hex(address - base_address).upper() == "0X6B0E":
        #     print("innoSecure result:")
        #     r4 = mu.reg_read(UC_ARM_REG_R4)
        #     hexdump(read_byte_array(mu, r4, 169))

        # if hex(address - base_address).upper() == "0X6B1A":
        #     print("innoSecure result length:")
        #     r0 = mu.reg_read(UC_ARM_REG_R0)
        #     print(hex(r0))

        # 结果的第一部分
        # 00000000  44 38 43 37 39 45 45 35 31 43 44 44 30 30 37 44  |D8C79EE51CDD007D|
        # 00000010  36 30 32 39 34 31 34 35 38 46 38 33 42 35 44 34  |602941458F83B5D4|
        # 来自48个字节的md5,需要注意的是，这儿十六进制数均为大写
        if hex(address - base_address).upper() == "0X6A08":
            print("first part before md5:")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            hexdump(read_byte_array(mu, r1, 48))
        # 00000000: 64 4C 4B 64 63 6D BB F3  6B 16 BF 64 66 11 F3 36  dLKdcm..k..df..6
        # 00000010: F2 01 EE 69 60 15 51 3A  C6 50 84 FA 9D B2 F3 A6  ...i`.Q:.P......
        # 00000020: 2F 67 D9 46 21 17 E3 E2  99 64 3D AD EE 54 7D AF  /g.F!....d=..T}.

        # https://gchq.github.io/CyberChef/#recipe=From_Hexdump()MD5()To_Upper_case('All')To_Hexdump(16,false,false,false)&input=MDAwMDAwMDA6IDY0IDRDIDRCIDY0IDYzIDZEIEJCIEYzICA2QiAxNiBCRiA2NCA2NiAxMSBGMyAzNiAgZExLZGNtLi5rLi5kZi4uNgowMDAwMDAxMDogRjIgMDEgRUUgNjkgNjAgMTUgNTEgM0EgIEM2IDUwIDg0IEZBIDlEIEIyIEYzIEE2ICAuLi5pYC5ROi5QLi4uLi4uCjAwMDAwMDIwOiAyRiA2NyBEOSA0NiAyMSAxNyBFMyBFMiAgOTkgNjQgM0QgQUQgRUUgNTQgN0QgQUYgIC9nLkYhLi4uLmQ9Li5UfS4

        # 验证第一部分这48个字节的来源
        # hook 点 结果不太匹配，原因未知
        if hex(address - base_address).upper() == "0X69AC":
            print("aes result:")
            r0 = mu.reg_read(UC_ARM_REG_R0)
            hexdump(read_byte_array(mu, r0, 16))

        # 不经过
        if hex(address - base_address).upper() == "0X6E4E":
            print("v187:")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            hexdump(read_byte_array(mu, r1, 32))

        #
        if hex(address - base_address).upper() == "0X64B0":
            print("v187:")
            r4 = mu.reg_read(UC_ARM_REG_R4)
            hexdump(read_byte_array(mu, r4, 32))


        # 00000000: F2 01 EE 69 60 15 51 3A  C6 50 84 FA 9D B2 F3 A6  ...i`.Q:.P......
        # 00000010: 2F 67 D9 46 21 17 E3 E2  99 64 3D AD EE 54 7D AF  /g.F!....d=..T}.
        # 可以发现，这就是48个字节的后32个，以此为依据，修改IDA的F5代码
        # 修改前
        #     v172[0] = *(_DWORD *)v168;
        #     v172[1] = v169;
        #     v172[2] = v170;
        #     v172[3] = v171;
        #     *(_DWORD *)v163 = *(_DWORD *)v168;          // 第一个16字节来自v172
        #     *(_DWORD *)&v163[4] = v169;
        #     *(_DWORD *)&v163[8] = v170;
        #     *(_DWORD *)&v163[12] = v171;
        #     *(_DWORD *)&v163[16] = *(_DWORD *)v187;     // 第二个16字节来自v187
        #     *(_DWORD *)&v163[20] = *(_DWORD *)&v187[4];
        #     *(_DWORD *)&v163[24] = *(_DWORD *)&v187[8];
        #     *(_DWORD *)&v163[28] = *(_DWORD *)&v187[12];
        #     v164 = *(_DWORD *)&v187[16];
        #     v165 = *(_DWORD *)&v187[20];
        #     v166 = *(_DWORD *)&v187[24];
        #     v167 = *(_DWORD *)&v187[28];
        # 其中v164-v167 无交叉引用
        # 修改后
        #     v168[0] = *(_DWORD *)v164;
        #     v168[1] = v165;
        #     v168[2] = v166;
        #     v168[3] = v167;
        #     *(_DWORD *)v163 = *(_DWORD *)v164;          // 第一个16字节来自v172
        #     *(_DWORD *)&v163[4] = v165;
        #     *(_DWORD *)&v163[8] = v166;
        #     *(_DWORD *)&v163[12] = v167;
        #     *(_DWORD *)&v163[16] = *(_DWORD *)v183;     // 第二个16字节来自v187
        #     *(_DWORD *)&v163[20] = *(_DWORD *)&v183[4];
        #     *(_DWORD *)&v163[24] = *(_DWORD *)&v183[8];
        #     *(_DWORD *)&v163[28] = *(_DWORD *)&v183[12];
        #     *(_DWORD *)&v163[32] = *(_DWORD *)&v183[16];
        #     *(_DWORD *)&v163[36] = *(_DWORD *)&v183[20];
        #     *(_DWORD *)&v163[40] = *(_DWORD *)&v183[24];
        #     *(_DWORD *)&v163[44] = *(_DWORD *)&v183[28];
        # 顺眼多了

        # 这样的话，48个字节就是来自两部分，而非一开始的伪代码那样，来自三部分了！
        # 第一部分交叉引用不太舒服，但我们动态调试发现就是aes的结果
        # https://gchq.github.io/CyberChef/#recipe=AES_Encrypt(%7B'option':'Hex','string':'FD22C75652058294AD0D241988AEF851E0254A86E9EE8F4FF67D739A2079B7CB'%7D,%7B'option':'Hex','string':'b8b16602faaf76d8c50afacc31dd882f'%7D,'CBC','Raw','Hex',%7B'option':'Hex','string':''%7D)&input=MDEyMzQ1Njc4OTEyMzQ
        # 第二部分哪里来的呢

        if hex(address - base_address).upper() == "0X64B0":
            print("第二部分的32个字节来源1:")
            r9 = mu.reg_read(UC_ARM_REG_R9)
            hexdump(read_byte_array(mu, r9, 32))
        # 00000000: F2 EE 60 51 C6 84 9D F3  2F D9 21 E3 99 3D EE 7D  ..`Q..../.!..=.}
        # 即奇数位


        if hex(address - base_address).upper() == "0X6402":
            print("uuid:")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            hexdump(read_byte_array(mu, r1, 32))
        # 奇数位来自于 uuid md5
        # https://gchq.github.io/CyberChef/#recipe=MD5()&input=YWY1NGFkNjQtZTIxNy00NjY3LWE2YjItZmE1MDNhMTU2OTAx

        if hex(address - base_address).upper() == "0X64AC":
            print("my r2:")
            r2 = mu.reg_read(UC_ARM_REG_R2)
            print(hex(r2))
        # 验证后确实是偶数位的值
        # 那么偶数位的值哪里来的呢

        if hex(address - base_address).upper() == "0X64A4":
            print("my r0:")
            r0 = mu.reg_read(UC_ARM_REG_R0)
            hexdump(read_byte_array(mu, r0, 32))

        if hex(address - base_address).upper() == "0X6494":
            print("my r6:")
            r6 = mu.reg_read(UC_ARM_REG_R6)
            hexdump(read_byte_array(mu, r6, 32))
        # AF 54 AD 64 E2 17 46 67  A6 B2 FA 50 3A 15 69 01

        # 偶数的来源
        # 进入x29前，获取最后一个参数 STRD.W R5, R6, [SP,#0x540+var_540]
        # 这个地址在出来后再看
        if hex(address - base_address).upper() == "0X6486":
            print("my r666 address:")
            r6 = mu.reg_read(UC_ARM_REG_R6)
            print(hex(r6))
        if hex(address - base_address).upper() == "0X648A":
            print("my r666 return contents:")
            hexdump(read_byte_array(mu, 0x100ffbe8, 16))
        # AF 54 AD 64 E2 17 46 67  A6 B2 FA 50 3A 15 69 01
        # j_innosecure_x29(length_uuid, (int)uuid, 1, 0, 0, (int)v174);

        # 我们意识到，奇数位是uuid md5后的结果，偶数位是uuid 的另一种作妖。
        # uuid 固定即不变

        if hex(address - base_address).upper() == "0X6AF2":
            print("0x13 part:")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            hexdump(read_byte_array(mu, r1, 0x13))
        # 00000000: 28 96 64 B5 A0 F9 AC C3  D4 14 D4 15 2C 24 D0 57  (.d.........,$.W
        # 00000010: 1F 99 AD

        if hex(address - base_address).upper() == "0X66E6":
            print("v179 part:")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            hexdump(read_byte_array(mu, r1, 0x100))

        # 00000000: 70 61 72 61 6D 25 63 25  73 25 63 76 65 72 73 69  param%c%s%cversi
        # 00000010: 6F 6E 25 63 25 73 25 63  70 6C 61 74 66 6F 72 6D  on%c%s%cplatform
        # 00000020: 25 63 61 6E 64 72 6F 69  64 25 63 65 63 25 63 31  %candroid%cec%c1
        # 00000030: 00 28 96 64 B5 A0 F9 AC  C3 D4 14 D4 15 2C 24 D0  .(.d.........,$.
        # 00000040: 57 1F 99 AD 00 00 00 00  00 B5 A0 F9 AC C3 D4 14  W...............
        # 00000050: D4 15 2C 24 D0 57 1F 99  AD B8 B1 66 02 FA AF 76  ..,$.W.....f...v
        # 00000060: D8 C5 0A FA CC 31 DD 88  2F F2 01 EE 69 60 15 51  .....1../...i`.Q
        # 00000070: 3A C6 50 84 FA 9D B2 F3  A6 2F 67 D9 46 21 17 E3  :.P....../g.F!..
        # 00000080: E2 99 64 3D AD EE 54 7D  AF 00 00 00 00 00 02 00  ..d=..T}........
        # 00000090: 00 00 00 00 00 F2 EE 60  51 C6 84 9D F3 2F D9 21  .......`Q..../.!
        # 000000A0: E3 99 3D EE 7D 61 66 35  34 61 64 36 34 2D 65 32  ..=.}af54ad64-e2
        # 000000B0: 31 37 2D 34 36 36 37 2D  61 36 62 32 2D 66 61 35  17-4667-a6b2-fa5
        # 000000C0: 30 33 61 31 35 36 39 30  31 80 00 00 00 00 00 00  03a156901.......
        # 000000D0: 00 00 00 00 00 00 00 00  00 00 00 00 00 20 01 00  ............. ..
        # 000000E0: 00 00 00 00 00 00 F2 EE  60 51 C6 84 9D F3 2F D9  ........`Q..../.
        # 000000F0: 21 E3 99 3D EE 7D 25 30  32 58 25 30 32 58 25 30  !..=.}%02X%02X%0

        if hex(address - base_address).upper() == "0X65FC":
            print("LR part:")
            lr = mu.reg_read(UC_ARM_REG_LR)
            print(hex(lr))
        # 28

        if hex(address - base_address).upper() == "0X6600":
            print("this R2:")
            r2 = mu.reg_read(UC_ARM_REG_R2)
            print(hex(r2))
        # 64

        if hex(address - base_address).upper() == "0X65F2":
            print("this R0:")
            r0 = mu.reg_read(UC_ARM_REG_R0)
            print(hex(r0))
        # 0xfffffe96
        # 因为取byte，即0x96

        # 0x19 个字节 前3个就是这么来的。


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
    a1 = "01234567891234"
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
