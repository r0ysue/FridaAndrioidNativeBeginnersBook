import logging
from androidemu.java.java_class_def import JavaClassDef
from androidemu.java.java_field_def import JavaFieldDef
from androidemu.java.java_method_def import java_method_def, JavaMethodDef
from androidemu.java.jni_ref import *

logger = logging.getLogger(__name__)

class ApplicationInfo(metaclass=JavaClassDef, jvm_name='android/content/pm/ApplicationInfo', 
jvm_fields=[
                     JavaFieldDef('sourceDir', 'Ljava/lang/String;', False),
                     JavaFieldDef('dataDir', 'Ljava/lang/String;', False),
                     JavaFieldDef('nativeLibraryDir', 'Ljava/lang/String;', False)
                 ]):
    
    def __init__(self):
        self.sourceDir = "/data/app/com.myxh.coolshopping/"
        self.dataDir = "/data/data/com.myxh.coolshopping/"
        self.nativeLibraryDir = "/data/data/com.myxh.coolshopping/lib/"
    #
#

class Signature(metaclass=JavaClassDef, jvm_name='android/content/pm/Signature'):
    def __init__(self):
        pass

    @java_method_def(name='getPackageManager', signature='()Landroid/content/pm/PackageManager;')
    def getPackageManager(self,mu):
        logger.info("Im in Signature.getPackageManager")
        packMag = PackageManager()
        return packMag

    @java_method_def(name='toByteArray', signature='()[B')
    def toByteArray(self,mu):
        logger.info("Im in Signature.toByteArray 3082")
        return bytearray.fromhex('308201e53082014ea003020102020455aa0da0300d06092a864886f70d01010505003037310b30090603550406130255533110300e060355040a1307416e64726f6964311630140603550403130d416e64726f6964204465627567301e170d3135303731383038323630385a170d3435303731303038323630385a3037310b30090603550406130255533110300e060355040a1307416e64726f6964311630140603550403130d416e64726f696420446562756730819f300d06092a864886f70d010101050003818d0030818902818100e22a46cb2a522abf704688bec41f5a63df3c1b1b8629ce618a73c86fc91f2d43bdc67d50dfe448d3a9b9578e4c69a92ed015fd262a2d849e007a12f26375ccaa49f16b18ae9e8ac1bc7f17d607d5aa33708b9911b626abaf0b69ef6ca46e4f9c471adeaaa6fd52fd6f0cbac481efe85f8424975b153c5238466351d559f70ab50203010001300d06092a864886f70d010105050003818100b82be22bf2a07e0ab0d0ddd2f8b875527264a823f0c2d230b446cb48c2db4cf28f3730a68e99645f753ae1917479136757523cad330fbd348290b35ea3fc8bb92f98fa17e2e16f0fb28079135163d980de6048e62fa0cd5a881ba3ae9dbc28beca69e4a344bf3ee98cde6faf50e892b49b0fba4d02ff5e4948c28a3b91e61bd6') 

    
class PackageInfo(metaclass=JavaClassDef, jvm_name='android/content/pm/PackageInfo', 
jvm_fields=[
            JavaFieldDef('applicationInfo', 'Landroid/content/pm/ApplicationInfo;', False),
            JavaFieldDef("signatures", "[Landroid/content/pm/Signature;", False),
           ]):
    def __init__(self):
        self.applicationInfo = ApplicationInfo()
        self.signatures = [Signature(),]
    #
#


class PackageManager(metaclass=JavaClassDef, jvm_name='android/content/pm/PackageManager'):
    def __init__(self):
        self.__pkg_info = PackageInfo()
    #

    @java_method_def(name='getPackageInfo', signature='(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;', native=False)
    def getPackageManager(self, emu):
        return self.__pkg_info
    #
#
