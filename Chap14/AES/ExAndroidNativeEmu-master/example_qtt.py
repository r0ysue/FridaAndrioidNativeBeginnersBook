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


# class android_content_pm_PackageInfo_signatures(metaclass=JavaClassDef, jvm_name='android/content/pm/PackageInfo',
#                                                 jvm_fields=[
#                                                     JavaFieldDef('signatures', '[Landroid/content/pm/Signature;', None),
#                                                 ]):
#     def __init__(self):
#         pass


class java_lang_System(metaclass=JavaClassDef, jvm_name='java/lang/System'):
    def __init__(self):
        pass

    @java_method_def(name='getProperty', args_list=["jstring"], signature='(Ljava/lang/String;)Ljava/lang/String;',
                     native=False)
    def getProperty(self, *args, **kwargs):
        print(args[0])
        return String("2.1.0")


class java_lang_StackTraceElement(metaclass=JavaClassDef, jvm_name='java/lang/StackTraceElement'):
    def __init__(self, _name):
        self.name = _name

    @java_method_def(native=False, name='getClassName', signature="()Ljava/lang/String;")
    def getClassName(self, *args, **kwargs):
        return self.name


class java_lang_Thread(metaclass=JavaClassDef, jvm_name='java/lang/Thread'):
    def __init__(self):
        pass

    @java_method_def(name="currentThread", signature='()Ljava/lang/Thread;', native=False)
    def currentThread(self, *args, **kwargs):
        return java_lang_Thread()

    @java_method_def(name="getStackTrace", signature='()[Ljava/lang/StackTraceElement;', native=False)
    def getStackTrace(self, *args, **kwargs):
        l = [java_lang_StackTraceElement(String("dalvik.system.VMStack")),
             java_lang_StackTraceElement(String("java.lang.Thread")),
             java_lang_StackTraceElement(String("com.ss.sys.ces.a")),
             java_lang_StackTraceElement(String("com.yf.douyintool.MainActivity")),
             java_lang_StackTraceElement(String("java.lang.reflect.Method")),
             java_lang_StackTraceElement(String("java.lang.reflect.Method")),
             java_lang_StackTraceElement(
                 String("android.support.v7.app.AppCompatViewInflater$DeclaredOnClickListener")),
             java_lang_StackTraceElement(String("android.view.View")),
             java_lang_StackTraceElement(String("android.os.Handler")),
             java_lang_StackTraceElement(String("android.os.Handler")),
             java_lang_StackTraceElement(String("android.os.Looper")),
             java_lang_StackTraceElement(String("android.app.ActivityThread")),
             java_lang_StackTraceElement(String("java.lang.reflect.Method")),
             java_lang_StackTraceElement(String("java.lang.reflect.Method")),
             java_lang_StackTraceElement(String("com.android.internal.os.ZygoteInit$MethodAndArgsCaller")),
             java_lang_StackTraceElement(String("com.android.internal.os.ZygoteInit")),
             java_lang_StackTraceElement(String("dalvik.system.NativeStart"))
             ]
        #
        #
        r = List(l)
        return r
    #


def hook_mem_read(uc, access, address, size, value, user_data):
    # mnt = user_data
    # pc = uc.reg_read(UC_ARM_REG_PC)
    print(hex(address), address)
    # addressr0 = emulator.mu.reg_read(UC_ARM_REG_R0)

    # 在之前申请的buff读取内存
    # detect_str = memory_helpers.read_utf8(emulator.mu, address)
    # print("detect_str: " + detect_str)
    # print(hex(address), addressr0, "0xCBBD131C")
    # print(address,int(base_address, 16) + int("0x631C", 16))
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
    # print(pc,access, address, size, value, user_data)
    # mnt.feed_read(pc, address, size)


def jb2pb(byte_arr):
    """
    java 字节数组转python字节数组
    :return:
    """

    return [i + 256 if i < 0 else i for i in byte_arr]


#

def hook_mem_write(uc, access, address, size, value, user_data):
    mnt = user_data
    pc = uc.reg_read(UC_ARM_REG_PC)
    # mnt.feed_write(pc, address, size)
    #



# g_cfd = ChainLogger(sys.stdout, "./ins-douyin.txt")


# Add debugging.
def hook_code(mu, address, size, user_data):
    try:
        emu = user_data
        if (not emu.memory.check_addr(address, UC_PROT_EXEC)):
            logger.error("addr 0x%08X out of range" % (address,))
            sys.exit(-1)
        # if hex(address - base_address).upper() == "0X6CE8":
        #     # print("hook  start ---------------------------\n")
        #     from androidemu.utils.memory_helpers import read_byte_array, read_ptr, read_utf8
        #     r1 = mu.reg_read(UC_ARM_REG_R1)
        #     print("mark")
        #     print(hex(r1))
        #     # print(hexdump(read_byte_array(mu, r1, 128)))
        #     # print("hook  end ---------------------------\n")
        # if hex(address - base_address).upper() == "0X6544":
        #     from androidemu.utils.memory_helpers import read_byte_array, read_ptr, read_utf8
        #     r0 = mu.reg_read(UC_ARM_REG_R0)
        #     print("6544")
        #     print(hex(r0))
        if hex(address - base_address).upper() == "0X6B04":
            r1 = mu.reg_read(UC_ARM_REG_R1)
            print("AES result")
            print(hexdump(read_byte_array(mu, r1, 128)))

        # if hex(address - base_address).upper() == "0X6E18":
        #     r1 = mu.reg_read(UC_ARM_REG_R1)
        #     print("AES result 111")
        #     print(hexdump(read_byte_array(mu, r1, 128)))

        #

        # S 盒替换前
        if hex(address - base_address).upper() == "0X6D76":
            r1 = mu.reg_read(UC_ARM_REG_R1)
            print("origin")
            print(hex(r1))

        # S盒替换后
        if hex(address - base_address).upper() == "0X6D7A":
            r1 = mu.reg_read(UC_ARM_REG_R1)
            print("new")
            print(hex(r1))

        # S盒
        if hex(address - base_address).upper() == "0X6D72":
            r2 = mu.reg_read(UC_ARM_REG_R2)
            print("S box")
            print(hexdump(read_byte_array(mu, r2, 256)))
            r11 = mu.reg_read(UC_ARM_REG_R11)
            print("message before S box")
            print(hexdump(read_byte_array(mu, r11, 256)))
        # if hex(address - base_address).upper() == "0X69B2":
        #     r5 = mu.reg_read(UC_ARM_REG_R5)
        #     print("AES result 1")
        #     print(hexdump(read_byte_array(mu, r5, 128)))
        # if hex(address - base_address).upper() == "0X6548":
        #     from androidemu.utils.memory_helpers import read_byte_array, read_ptr, read_utf8
        #     r0 = mu.reg_read(UC_ARM_REG_R0)
        #     print("6548")
        #     print(hex(r0))

        if hex(address - base_address).upper() == "0X6BA4":
            r0 = mu.reg_read(UC_ARM_REG_R0)
            print("plainText :")
            print(hexdump(read_byte_array(mu, r0, 128)))

        # if hex(address - base_address).upper() == "0X6BC8":
        #     print("Second")
        #     # print("6bc8")
        #     r1 = mu.reg_read(UC_ARM_REG_R1)
        #     r2 = mu.reg_read(UC_ARM_REG_R2)
        #     print(hex(r1))
        #     print(hex(r2))

        # 初始化IV hook
        # if hex(address - base_address).upper() == "0X6CEA":
        #     print("First")
        #     # print("6bc8")
        #     r1 = mu.reg_read(UC_ARM_REG_R1)
        #     r2 = mu.reg_read(UC_ARM_REG_R2)
        #     print(hex(r1))
            # print(hex(r2))

        # 疑似密钥
        if hex(address - base_address).upper() == "0X6D06":
            print("0X6D06")
            r2 = mu.reg_read(UC_ARM_REG_R2)
            # r3 = mu.reg_read(UC_ARM_REG_R3)
            print(hex(r2))
            # print(hex(r3))

        if hex(address - base_address).upper() == "0X6F9A":
            print("AES result")
            r1 = mu.reg_read(UC_ARM_REG_R1)
            r2 = mu.reg_read(UC_ARM_REG_R2)
            # print(hex(r1))
            print(hex(r2))
            print(hexdump(read_byte_array(mu, r1, 128)))
        # androidemu.utils.debug_utils.dump_code(emu, address, size, sys.stdout)

        if hex(address - base_address).upper() == "0X6D34":
            print("HIT 2!!!!!!!! ")

        if hex(address - base_address).upper() == "0X6D54":
            print("HIT 1!!!!!!!! ")

        # 调用13次
        if hex(address - base_address).upper() == "0X2A8A":
            print("x11")
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
        logger.info("base_address=> 0x%08x - %s" % (module.base, module.filename))

        break
# emulator.mu.hook_add(UC_HOOK_MEM_READ, hook_mem_read)


emulator.mu.hook_add(UC_HOOK_CODE, hook_code, emulator)

# a1 = "{\"dtu\":\"014\",\"lon\":\"0.0\",\"tk\":\"ACHpZhpqUc9JqL7fXDqjtJkfY7jxKzu_DfA0NzUxNDk1MDg5NTIyNQ\",\"time\":\"1620020548635\",\"guid\":\"\",\"token\":\"2b8eVnlKxD6LaGDETOuXXDPAUgU0OlB8zv6ejwU7D7aQlWKNeAUfuMsMM0rphCNY9QkjOkh9uCCB12ODYw8\",\"uuid\":\"971c7df2483443b693d3a7a3bd0cdd13\",\"tuid\":\"6WYaalHPSai-31w6o7SZHw\",\"contentID\":\"1623496401\",\"tasktype\":\"article\",\"env\":\"qukan_test\",\"OSVersion\":\"6.0.1\",\"network\":\"wifi\",\"versionName\":\"3.9.95.000.0909.1541\",\"video_duration\":0,\"distinct_id\":\"adef241b8feba2e7\",\"version\":\"3995000\",\"reportType\":0,\"lat\":\"0.0\"}"
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

""" ....t.......(Ljava/lang/String;)[B.getPackageName.()Ljava/lang/String;.getPackageInfo.(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;.[Landroid/content/pm/Signature;.toCharsString.secure.(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[B)[B.decode.([BLjava/lang/String;[B)[B.getn.getv.([B)Ljava/lang/String;.ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%0

"""
