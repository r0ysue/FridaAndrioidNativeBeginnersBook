package com.r0ysue.easyso;

import com.github.unidbg.AndroidEmulator;
import com.github.unidbg.Emulator;
import com.github.unidbg.LibraryResolver;
import com.github.unidbg.Module;
import com.github.unidbg.arm.HookStatus;
import com.github.unidbg.arm.backend.DynarmicFactory;
import com.github.unidbg.hook.HookContext;
import com.github.unidbg.hook.ReplaceCallback;
import com.github.unidbg.hook.hookzz.Dobby;
import com.github.unidbg.linux.android.AndroidEmulatorBuilder;
import com.github.unidbg.linux.android.AndroidResolver;
import com.github.unidbg.linux.android.dvm.*;
import com.github.unidbg.linux.android.dvm.jni.ProxyDvmObject;
import com.github.unidbg.memory.Memory;

import java.io.File;

/**
 * https://bbs.pediy.com/thread-263345.htm
 */
public class MainActivity extends AbstractJni {

    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        com.r0ysue.easyso.MainActivity mainActivity = new com.r0ysue.easyso.MainActivity();
        System.out.println("load offset=" + (System.currentTimeMillis() - start) + "ms");
        mainActivity.crack();
    }

    private final AndroidEmulator emulator;
    private final VM vm;
    private final DvmClass dvmClass;
    private String className = "com/roysue/easyso1/MainActivity";
    private final Module module;

    private MainActivity() {
        emulator = AndroidEmulatorBuilder
                .for32Bit()
//                .addBackendFactory(new DynarmicFactory(true))
                .build();
        Memory memory = emulator.getMemory();
        LibraryResolver resolver = new AndroidResolver(23);
        memory.setLibraryResolver(resolver);

        vm = emulator.createDalvikVM(null);
        System.out.println(vm);
        vm.setVerbose(true);
        vm.setJni(this);
        DalvikModule dm = vm.loadLibrary(new File("unidbg-android/src/test/resources/example_binaries/armeabi-v7a/libroysue.so"), false);

        module = dm.getModule();

        Dobby dobby = Dobby.getInstance(emulator);
        // 2. 使用ida pro查看导出方法名，尝试hook
        dobby.replace(module.findSymbolByName("_Z24function_check_tracerPIDv"), new ReplaceCallback() { // 使用Dobby inline hook导出函数
            @Override
            // 3. contextk可以拿到参数，originFunction是原方法的地址
            public HookStatus onCall(Emulator<?> emulator, HookContext context, long originFunction) {
//                System.out.println("create_thread_check_traceid.onCall function address => 0x" + Long.toHexString(originFunction));
                System.out.println("calling _Z14function_checkv ....");
//                return HookStatus.RET(emulator, originFunction);
//                return null;
                return HookStatus.LR(emulator, 0);
            }
            @Override

            public void postCall(Emulator<?> emulator, HookContext context) {
                System.out.println(" calling _Z14function_checkv .... return false");

//                context.getIntArg(0) ;

            }
        }, false);




        dm.callJNI_OnLoad(emulator);
        dvmClass = vm.resolveClass(className);
    }


    private void crack() {

        DvmObject result =  dvmClass.callStaticJniMethodObject(emulator,"Sign(Ljava/lang/String;)Ljava/lang/String;","requestUserInfo");
//        long start = System.currentTimeMillis();

        System.out.println("final result is => "+ result.getValue());
    }
}
