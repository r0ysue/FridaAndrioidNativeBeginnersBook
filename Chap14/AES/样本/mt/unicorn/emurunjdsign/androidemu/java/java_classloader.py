import logging
from androidemu.java.java_class_def import JavaClassDef
from androidemu.java.classes.clazz import Class

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


class JavaClassLoader(metaclass=JavaClassDef, jvm_name='java/lang/ClassLoader'):

    """
    :type class_by_id dict[int, JavaClassDef]
    :type class_by_name dict[string, JavaClassDef]
    """
    def __init__(self):
        self.class_by_id = dict()
        self.class_by_name = dict()

    def add_class(self, clazz):
        if not isinstance(clazz, JavaClassDef):
            raise ValueError('Expected a JavaClassDef.')
        #
        if clazz.jvm_name in self.class_by_name:
            raise KeyError('The class \'%s\' is already registered.' % clazz.jvm_name)
        #
        if (clazz == Class):
            clazz.class_loader = self
        #
        clazz.class_object = Class(clazz.jvm_name)
        self.class_by_id[clazz.jvm_id] = clazz
        self.class_by_name[clazz.jvm_name] = clazz
        logger.debug("add_class = %s",clazz.jvm_name)

        for key in clazz.jvm_methods.keys():
            logger.debug("%d,%s"% (key , clazz.jvm_methods[key].name) )

    #

    def find_class_by_id(self, jvm_id):
        if jvm_id not in self.class_by_id:
            return None

        return self.class_by_id[jvm_id]

    def find_class_by_name(self, name):
        if name not in self.class_by_name:
            return None

        return self.class_by_name[name]
