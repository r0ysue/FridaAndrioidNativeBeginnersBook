import inspect
import itertools
import logging
from androidemu.java.jvm_id_conter import *

logger = logging.getLogger(__name__)

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


class JavaClassDef(type):
    
    def __init__(cls, name, base, ns, jvm_name=None, jvm_fields=None, jvm_ignore=False, jvm_super=None):
        cls.jvm_id = next_cls_id()
        cls.jvm_name = jvm_name
        cls.jvm_methods = dict()
        cls.jvm_fields = dict()
        cls.jvm_ignore = jvm_ignore
        cls.jvm_super = jvm_super
        cls.class_object = None

        # Register all defined Java methods.
        for func in inspect.getmembers(cls, predicate=inspect.isfunction):
            if hasattr(func[1], 'jvm_method'):
                method = func[1].jvm_method
                cls.jvm_methods[method.jvm_id] = method
                logger.debug("JavaClassDef method.jvm_id = %s, name = %s"  % ( method.jvm_id , method.name))
        #
        # Register all defined Java fields.
        if jvm_fields is not None:
            for jvm_field in jvm_fields:
                cls.jvm_fields[jvm_field.jvm_id] = jvm_field
        #
        type.__init__(cls, name, base, ns)
    #

    def __new__(cls, name, base, ns, **kargs):
        return type.__new__(cls, name, base, ns)
    #

    def register_native(cls, name, signature, ptr_func):
        found = False
        found_method = None

        # Search for a defined jvm method.
        for method in cls.jvm_methods.values():
            if method.name == name and method.signature == signature:
                method.native_addr = ptr_func
                found = True
                found_method = method
                break

        if not found:
            x = "Register native ('%s', '%s') failed on class %s." % (name, signature, cls.__name__)
            logger.warning(x)
            return
            # raise RuntimeError("Register native ('%s', '%s') failed on class %s." % (name, signature, cls.__name__))
        logger.debug("Registered native function ('%s', '%s') to %s.%s" % (name, signature,
                                                                           cls.__name__, found_method.func_name))

    def find_method(cls, name, signature):
        logger.debug("find_method (%s, %s,%s)" % (cls.__name__, name , signature) )
        
        for method in cls.jvm_methods.values():
            logger.debug(">>> %s,%s" % (method.name,method.signature))
            if method.name == name and method.signature == signature:
                logger.debug("find_method OK (%s, %s,%s,%s)" % (cls.__name__, name , signature, method.jvm_id) )
                return method
            #
        #
        return None

    def find_method_by_id(cls, jvm_id):
        try:
            if cls.jvm_super is not None:
                return cls.jvm_super.find_method_by_id(jvm_id)
            #
        #
        except KeyError:
            pass
        #

        logger.debug("find_method_by_id (%s, %s)" % (cls.__name__, jvm_id) )
        
        for key in cls.jvm_methods.keys():
            logger.debug("%d,%s"% (key , cls.jvm_methods[key]) )
            
        return cls.jvm_methods[jvm_id]

    def find_field(cls, name, signature, is_static):
        logger.debug("find_field (%s, %s,%s,%d)" % (cls.__name__, name , signature,is_static) )
        
        for field in cls.jvm_fields.values():
            if field.name == name and field.signature == signature and field.is_static == is_static:
                return field

        return None

    def find_field_by_id(cls, jvm_id):
        try:
            if cls.jvm_super is not None:
                return cls.jvm_super.find_field_by_id(jvm_id)
        except KeyError:
            pass

        return cls.jvm_fields[jvm_id]
