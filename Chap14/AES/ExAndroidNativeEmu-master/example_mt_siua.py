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

class DataCollectionJni(metaclass=JavaClassDef, jvm_name='com/meituan/android/common/datacollection/DataCollectionJni'):
    def __init__(self):
        pass

    @java_method_def(name='packData',
                     signature='(Landroid/content/Context;[BI)[B',
                     native=True)
    def packData(self, mu):
        pass



logger = logging.getLogger(__name__)

g_cfd = ChainLogger(sys.stdout, "./ins-siua.txt")

emulator = Emulator(
    vfp_inst_set=True,
    vfs_root=posixpath.join(posixpath.dirname(__file__), "vfs")
)

emulator.java_classloader.add_class(DataCollectionJni)


lib_module = emulator.load_library("tests/bin/libmtguard.so")

for module in emulator.modules:
    if "libmtguard" in module.filename:
        base_address = module.base
        base_size = module.size
        logger.info("base_address=> 0x%08x - %s" % (module.base, module.filename))

        break

# Add debugging.
def hook_code(mu, address, size, user_data):
    try:
        emu = user_data
        if (not emu.memory.check_addr(address, UC_PROT_EXEC)):
            logger.error("addr 0x%08X out of range" % (address,))
            sys.exit(-1)


        # code trace
        # if address in range(base_address, base_address+base_size):
        #     androidemu.utils.debug_utils.dump_code(emu, address, size, g_cfd)
    except:
        pass

emulator.mu.hook_add(UC_HOOK_CODE, hook_code, emulator)

emulator.call_symbol(lib_module, 'JNI_OnLoad', emulator.java_vm.address_ptr, 0x00)


arr = [48]
length = 1
arr = Array(bytearray(arr))
dataClass = DataCollectionJni()
result = dataClass.packData(emulator, 0, arr, length)