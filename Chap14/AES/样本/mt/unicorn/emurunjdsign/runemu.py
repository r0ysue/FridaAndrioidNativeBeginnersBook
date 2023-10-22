#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
# Python 2.x & 3.x compatible
# from distutils.log import warn as print

'''
 * <pre>
 * Created by fenfei
 * http://91fans.com.cn/
 * public accounts: fenfei330
 * wx:fenfei331  mail:fenfei331@126.com
 * data ：2020/11/21  First snow in beijing 
 * <pre>
 * ********************************我佛慈悲*********************************
 * **                              _oo0oo_                               **
 * **                             o8888888o                              **
 * **                             88" . "88                              **
 * **                             (| -_- |)                              **
 * **                             0\  =  /0                              **
 * **                           ___/'---'\___                            **
 * **                        .' \\\|     |// '.                          **
 * **                       / \\\|||  :  |||// \\                        **
 * **                      / _ ||||| -:- |||||- \\                       **
 * **                      | |  \\\\  -  /// |   |                       **
 * **                      | \_|  ''\---/''  |_/ |                       **
 * **                      \  .-\__  '-'  __/-.  /                       **
 * **                    ___'. .'  /--.--\  '. .'___                     **
 * **                 ."" '<  '.___\_<|>_/___.' >'  "".                  **
 * **                | | : '-  \'.;'\ _ /';.'/ - ' : | |                 **
 * **                \  \ '_.   \_ __\ /__ _/   .-' /  /                 **
 * **            ====='-.____'.___ \_____/___.-'____.-'=====             **
 * **                              '=---='                               **
 * ************************************************************************
 * **                佛祖保佑      镇类之宝       永无bug                   **
 * ************************************************************************
'''


import logging
import posixpath
import sys

from unicorn import *
from unicorn.arm_const import *

from androidemu.emulator import Emulator
from androidemu.java.java_class_def import JavaClassDef
from androidemu.java.java_field_def import JavaFieldDef
from androidemu.java.java_method_def import java_method_def
from androidemu.java.jni_env import JNIEnv
from androidemu.java.classes.package_manager import *
from androidemu.java.classes.activity_thread import *

import androidemu.config
import androidemu.utils.debug_utils
from androidemu.utils.chain_log import ChainLogger

import capstone
import traceback

from flask import Flask, jsonify, request

from UnicornDebugger import UnicornDebugger,REG_ARM

if sys.version_info < (3, 0):
    reload(sys)
    sys.setdefaultencoding('utf-8')

# Configure logging
logging.basicConfig(
    stream=sys.stdout,
    level=logging.DEBUG,
    format="%(asctime)s %(levelname)7s %(name)34s | %(message)s"
)

logger = logging.getLogger(__name__)

g_cfd = ChainLogger(sys.stdout, "./fenfei-jni.txt")
# Add debugging.
def hook_code(mu, address, size, user_data):
    try:
        emu = user_data
        if (not emu.memory.check_addr(address, UC_PROT_EXEC)):
            logger.error("addr 0x%08X out of range"%(address,))
            sys.exit(-1)
        #
        # androidemu.utils.debug_utils.dump_registers(mu, sys.stdout)
        # androidemu.utils.debug_utils.dump_code(emu, address, size, g_cfd)
    except Exception as e:
        logger.exception("exception in hook_code")
        sys.exit(-1)
    #
#

def hook_mem_read(uc, access, address, size, value, user_data):
    pc = uc.reg_read(UC_ARM_REG_PC)    
    pass

def hook_mem_write(uc, access, address, size, value, user_data):
    pc = uc.reg_read(UC_ARM_REG_PC)
    pass

# Application 和 Activity 的getPackageManager jvm_id 一样
class Application(metaclass=JavaClassDef, jvm_name='android/app/Application'):
    def __init__(self):
        self.__pkg_Manager = PackageManager()
        pass

    @java_method_def(name='getPackageManager', signature='()Landroid/content/pm/PackageManager;' , native=False,jvm_id=0xd2000000+0x1000)    
    def getPackageManager(self, mu):
        logger.info("Im in Application.getPackageManager")
        return self.__pkg_Manager

    @java_method_def(name='getPackageName', signature='()Ljava/lang/String;' , native=False,jvm_id=0xd2000000+0x1004)
    def getPackageName(self, mu):
        logger.info("Im in Application.getPackageName")
        return "com.jingdong.app.mall"
    
class Activity(metaclass=JavaClassDef, jvm_name='android/app/Activity'):
    def __init__(self):
        self.__pkg_Manager = PackageManager()
        pass

    @java_method_def(name='getPackageManager', signature='()Landroid/content/pm/PackageManager;' , native=False,jvm_id=0xd2000000+0x1000)
    def getPackageManager(self, mu):
        logger.info("Im in Activity.getPackageManager")
        return self.__pkg_Manager

    @java_method_def(name='getPackageName', signature='()Ljava/lang/String;' , native=False,jvm_id=0xd2000000+0x1004)
    def getPackageName(self, mu):
        logger.info("Im in Activity.getPackageName")
        return "com.jingdong.app.mall"

class Integer(metaclass=JavaClassDef, jvm_name='java/lang/Integer'):
    def __init__(self):
        self._int = -1
        pass

    @java_method_def(name='<init>', args_list=["jint"], signature='(I)V', native=False)
    def init(self, emu,*args, **kwargs):
        # pyobj = JNIEnv.jobject_to_pyobject(args[0])
        logger.info("Integer init %d" % args[0])
        self._int = args[0]
        pass

    @java_method_def(name='toString', signature='()Ljava/lang/String;', native=False)
    def toString(self, emu):
        logger.info("Integer toString %d" % self._int)
        return str(self._int)
    
class String(metaclass=JavaClassDef, jvm_name='java/lang/String'):
    def __init__(self,inStr=''):
        self._str = inStr
        pass

    @java_method_def(name='getBytes', args_list=["jstring"], signature='(Ljava/lang/String;)[B', native=False)
    def getBytes(self, emu,*args, **kwargs):
        logger.info("%r String getBytes %r" % (self._str, args) )
        return bytearray(self._str.encode('utf8'))

    @java_method_def(name='toString', signature='()Ljava/lang/String;', native=False)
    def toString(self, emu):
        return self._str
    
class StringBuffer(metaclass=JavaClassDef, jvm_name='java/lang/StringBuffer'):
    def __init__(self):
        self._str = ''
        pass
    
    @java_method_def(name='<init>', signature='()V', native=False)
    def init(self, emu):
        pass

    @java_method_def(name='append', args_list=["jstring"], signature='(Ljava/lang/String;)Ljava/lang/StringBuffer;', native=False)
    def append(self, emu,*args, **kwargs):
        logger.info("append %r" % args)
        pyobj = JNIEnv.jobject_to_pyobject(args[0])
        self._str += pyobj
        return self

    @java_method_def(name='toString', signature='()Ljava/lang/String;', native=False)
    def toString(self, emu):
        logger.info("toString %r" % self._str)
        return String(self._str)
    
class ToastUtils(metaclass=JavaClassDef, jvm_name='com/jingdong/jdsdk/widget/ToastUtils'):
    def __init__(self):
        pass

    @java_method_def(name='longToast',args_list=["jstring"], signature='(Ljava/lang/String;)V')
    def longToast(self, mu, *args, **kwargs):
        logger.info("longToast %r" % args)
        pass
    
class BitmapkitUtils(metaclass=JavaClassDef, jvm_name='com/jingdong/common/utils/BitmapkitUtils',jvm_fields=[JavaFieldDef("a", "Landroid/app/Application;", True, Application())]):

    def __init__(self):
        pass

    @java_method_def(name='a', signature='([Ljava/lang/String;)Ljava/lang/String;', native=True)
    def a(self, mu,str):
        logger.info("calll BitmapKitUtils a %s" % str)
        pass

    @java_method_def(name='encodeJni', signature='([BZ)[B', native=True)
    def encodeJni(self, mu):
        pass

    @java_method_def(
        name='getSignFromJni',
        args_list=['jobject',"jstring","jstring","jstring","jstring","jstring"],
        signature='(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;',
        native=True)
    def getSignFromJni(self, mu):
        pass

    @java_method_def(name='getstring', signature='(Ljava/lang/String;)Ljava/lang/String;', native=True)
    def getstring(self, mu):
        pass

global emulator
global lib_module
    
# 起个服务    
app = Flask(__name__)
@app.route('/getSignFromJni', methods=['POST']) # 数据加密
def getSignFromJni():
    global emulator
    global lib_module

    str = request.form.get('str')
    str2 = request.form.get('str2')
    str3 = request.form.get('str3')
    str4 = request.form.get('str4')
    str5 = request.form.get('str5')
    # data = request.get_data()
    # 对数据进行处理
    # print(data.decode("utf-8"))        

    activity_Th = ActivityThread()
    bit_Utils = BitmapkitUtils()
    result = emulator.call_symbol(lib_module, 'Java_com_jingdong_common_utils_BitmapkitUtils_getSignFromJni',emulator.java_vm.jni_env.address_ptr,0x00, activity_Th.getSystemContext(emulator), str, str2,str3, str4,str5)

    return result.toString(emulator)


def main():
    global emulator
    global lib_module
    
    filename = "./libjdbitmapkit.so"
    
    # Initialize emulator
    emulator = Emulator(
        vfp_inst_set = True,
        vfs_root=posixpath.join(posixpath.dirname(__file__), "vfs")
    )

    # Register Java class.
    emulator.java_classloader.add_class(Application)
    emulator.java_classloader.add_class(Activity)
    emulator.java_classloader.add_class(StringBuffer)
    emulator.java_classloader.add_class(String)
    emulator.java_classloader.add_class(Integer)
    
    emulator.java_classloader.add_class(BitmapkitUtils)
    emulator.java_classloader.add_class(ToastUtils)
    
    emulator.mu.hook_add(UC_HOOK_CODE, hook_code, emulator)

    emulator.mu.hook_add(UC_HOOK_MEM_WRITE, hook_mem_write)
    emulator.mu.hook_add(UC_HOOK_MEM_READ, hook_mem_read)

    # Load all libraries.
    lib_module = emulator.load_library(filename)

    # androidemu.utils.debug_utils.dump_symbols(emulator, sys.stdout)

    # Show loaded modules.
    logger.info("Loaded modules:")

    for module in emulator.modules:
        logger.info("=> 0x%08x - %s" % (module.base, module.filename))

    logger.info(">>> libjdbitmapkit.so load_base = 0x%x" % (lib_module.base) )
        
    try:
        
        # udbg = UnicornDebugger(emulator.mu)
        # udbg.add_bpt(lib_module.base + 0x12A6E)

        # Run JNI_OnLoad.
        # JNI_OnLoad will call 'RegisterNatives'.
        emulator.call_symbol(lib_module, 'JNI_OnLoad', emulator.java_vm.address_ptr, 0x00)

        # 第一种方法，直接调用
        activity_Th = ActivityThread()
        bit_Utils = BitmapkitUtils()
        result = emulator.call_symbol(lib_module, 'Java_com_jingdong_common_utils_BitmapkitUtils_getSignFromJni',emulator.java_vm.jni_env.address_ptr,0x00, activity_Th.getSystemContext(emulator), "search", '{"addrFilter":"1","addressId":"0","articleEssay":"1","deviceidTail":"71","exposedCount":"0","gcLat":"40.062931","gcLng":"116.362676","imagesize":{"gridImg":"354x354","listImg":"260x260","longImg":"354x453"},"insertArticle":"1","insertScene":"1","insertedCount":"0","isCorrect":"1","keyword":"苹果","latitude":"0.0","locLat":"","locLng":"","longitude":"0.0","newMiddleTag":"1","newVersion":"3","oneBoxMod":"1","orignalSearch":"1","orignalSelect":"1","page":"1","pageEntrance":"1","pagesize":"10","pvid":"","searchVersionCode":"9180","secondInsedCount":"0","showShopTab":"yes","showStoreTab":"1","stock":"1"}', "9900117606287-f460e11c02d8", "android", "9.2.2")
        
        logger.info("Response from JNI call: %s" % result.toString(emulator))

        # 启动web服务
        app.run()        
        
        # Dump natives found.
        logger.info("Exited EMU.")

    except UcError as e:
        logger.info("Exit at 0x%x" % emulator.mu.reg_read(UC_ARM_REG_PC))
        raise

    
if __name__ == '__main__':
    try:
        main()
        
    except KeyboardInterrupt:
        if session:
            session.detach()
        sys.exit()
    else:
        pass
    finally:
        pass
