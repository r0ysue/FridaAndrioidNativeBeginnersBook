from androidemu.java.classes.array import Array
from androidemu.java.classes.list import List
from ..java_class_def import JavaClassDef
from ..java_field_def import JavaFieldDef
from ..java_method_def import java_method_def, JavaMethodDef
from .string import String
import time


class ApplicationInfo(metaclass=JavaClassDef, jvm_name='android/content/pm/ApplicationInfo',
                      jvm_fields=[
                          JavaFieldDef('sourceDir', 'Ljava/lang/String;', False),
                          JavaFieldDef('dataDir', 'Ljava/lang/String;', False),
                          JavaFieldDef('nativeLibraryDir', 'Ljava/lang/String;', False),
                          JavaFieldDef('flags', 'I', False),
                      ]):

    def __init__(self, pyPkgName):
        self.sourceDir = String("/data/app/%s-1.apk" % pyPkgName)
        self.dataDir = String("/data/data/%s" % pyPkgName)
        self.nativeLibraryDir = String("/data/data/%s" % pyPkgName)
        self.flags = 0x30e8bf46
    #


class Signature(metaclass=JavaClassDef, jvm_name='android/content/pm/Signature'):
    def __init__(self):
        pass

    @java_method_def(name='toCharsString', signature='()Ljava/lang/String;')
    def toByteArray(self, mu):
        a3 = "3082021b30820184a0030201020204574beab6300d06092a864886f70d01010505003052310c300a06035504061303303231310b3009060355040813025348310b3009060355040713025348310b3009060355040a13025a48310b3009060355040b1302434e310e300c0603550403130551754b616e301e170d3136303533303037323433385a170d3431303532343037323433385a3052310c300a06035504061303303231310b3009060355040813025348310b3009060355040713025348310b3009060355040a13025a48310b3009060355040b1302434e310e300c0603550403130551754b616e30819f300d06092a864886f70d010101050003818d0030818902818100aa5bae49b771380e692444437b82b375cabdefb3f23307c29510653776b8e4115f776bea5eb6690285f97d4e6e8d0469e49f79ecba31e4b7fb85dd612ee6b27ef38502aa38d055ddad2aa7b52d19fb8d2aeeb59a830b91c341f1b467655e7313e9ff65feb6539bf1655f35a37e17faa94e506a08219df196730f45d9c1cd94d30203010001300d06092a864886f70d0101050500038181000e6cc9fb74aef11dd33d6603869a9db61b8dcedae77bc815433026693fe59fd4b75a3284170f8872737e55595c1fd40da3dfbe5ad8a4e96802f53637977f0eb6e9b0dc35161cbaed398b41ecd73c4009a1dae7bcb00b75c3f8d5792405bcc5e4602d9dff6a0dc4739240a3b42626f5efce4d7baea0fced2b13361cb4ded8ed0b"
        return String(a3)


class PackageInfo(metaclass=JavaClassDef, jvm_name='android/content/pm/PackageInfo',
                  jvm_fields=[
                      JavaFieldDef('applicationInfo', 'Landroid/content/pm/ApplicationInfo;', False),
                      JavaFieldDef('firstInstallTime', 'J', False),
                      JavaFieldDef('lastUpdateTime', 'J', False),
                      JavaFieldDef('signatures', '[Landroid/content/pm/Signature;', False),
                  ]):
    def __init__(self, pyPkgName):
        self.applicationInfo = ApplicationInfo(pyPkgName)
        self.firstInstallTime = int(time.time())
        self.lastUpdateTime = self.firstInstallTime
        self.signatures = List([Signature()])


class PackageManager(metaclass=JavaClassDef, jvm_name='android/content/pm/PackageManager'):
    def __init__(self, pyPkgName):
        self.__pkg_info = PackageInfo(pyPkgName)

    #

    @java_method_def(name='getPackageInfo', signature='(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;',
                     native=False)
    def getPackageInfo(self, emu):
        return self.__pkg_info
    #
#
