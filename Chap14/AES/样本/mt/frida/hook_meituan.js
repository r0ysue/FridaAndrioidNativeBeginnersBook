// 找到RegisterNatives函数的地址
function hook_libart() {
    // 采用枚举的方式找到模块
    var module_libart = Process.findModuleByName("libart.so");
    //枚举模块中的所有符号
    var symbols = module_libart.enumerateSymbols();
    var RegisterNatives_addr = null;
    var GetStringUTFChars_addr = null;
    var NewStringUTF_addr = null;
    var FindClass_addr = null
    var kill_addr = null;
    for (var i = 0; i < symbols.length; i++) {
        var name = symbols[i].name;
        if (name.indexOf("CheckJNI") == -1 && name.indexOf("JNI") > 0) {
            if (name.indexOf("RegisterNatives") > 0) {
                console.log(name)
                // 找到符的条件的符号保存地址
                RegisterNatives_addr = symbols[i].address;
            } else if (name.indexOf("GetStringUTFChars") >= 0) {
                console.log(name)
                GetStringUTFChars_addr = symbols[i].address;
            } else if (name.indexOf("NewStringUTF") >= 0) {
                console.log(name)
                NewStringUTF_addr = symbols[i].address;
            } else if (name.indexOf("FindClass") >= 0) {
                console.log(name)
                FindClass_addr = symbols[i].address;
            } else if (name.indexOf("kill") >= 0) {
                console.log(name)
                kill_addr = symbols[i].address;
            }
        }
    }

    if (RegisterNatives_addr) {
        // attach 方式hook
        /*
        static jint RegisterNatives(JNIEnv* env,
            jclass java_class,
            const JNINativeMethod* methods,
            jint method_count
        )
        */
        Interceptor.attach(RegisterNatives_addr, {
            onEnter: function (args) {
                // 使用Java.vm.tryGetEnv().getClassName获取jclass类名
                var jclass = Java.vm.tryGetEnv().getClassName(args[1])
                var methods = args[2]
                // 将数字转换成Int
                var method_count = parseInt(args[3])
                console.log("RegisterNatives_addr jclass :", jclass, args[2], "method_count:", method_count);
                // 获取所有动态注册的方法
                for (var i = 0; i < method_count; i++) {
                    // 处理偏移 因为JNINativeMethod 的结构体含有3个指针所以需要乘以3个指针长度
                    /*
                    typedef struct {
                        char *name;
                        char *signature;
                        void *fnPtr;
                    } JNINativeMethod;
                    */
                    // 函数名称
                    console.log(methods.add(i * Process.pointerSize * 3).readPointer().readCString());
                    // 签名
                    console.log(methods.add(i * Process.pointerSize * 3 + Process.pointerSize).readPointer().readCString());
                    // 地址
                    var fnPtr = methods.add(i * Process.pointerSize * 3 + Process.pointerSize * 2).readPointer();
                    //Process.findModuleByAddress 通过函数地址寻找module
                    //module.base 模块基址
                    var find_module = Process.findModuleByAddress(fnPtr);
                    console.log(find_module.name + "!" + fnPtr.sub(find_module.base));
                }
            }, onLeave: function (retval) {

            }
        })
    }
}



// 打印hex或地址
function hex_dump(p) {
    try {
        return hexdump(p) + "\r\n";
    } catch (error) {
        return ptr(p) + "\r\n";
    }
}
// hook
function hook_native_addr(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.arg0 = args[0];
            this.arg1 = args[1];
            this.arg2 = args[2];
            this.arg3 = args[3];
            // 获得函数的返回地址  64位寄存器是lr
            this.lr = this.context.lr; 
        }, onLeave: function (retval) {
            // console.log(JSON.stringify(this.context))
            console.log("ptr:" + ptr(real_addr) + "idb_addr:" + ptr(offset_addr) + " LR:" + ptr(this.lr).sub(base_addr) + " \r\n",
                "this.arg0:\r\n", hex_dump(this.arg0),
                "this.arg1:\r\n", hex_dump(this.arg1),
                "this.arg2:\r\n", hex_dump(this.arg2),
                "this.arg3:\r\n", hex_dump(this.arg3),
                "retval:\r\n", hex_dump(retval)
                );


                console.log('CCCryptorCreate called from:\n' +
        Thread.backtrace(this.context, Backtracer.ACCURATE)
        .map(DebugSymbol.fromAddress).join('\n') + '\n');
        }
    });
}

function hook_native() {
    var base_addr = Module.findBaseAddress("libmtguard.so");
    
    // hook_native_addr(base_addr, 0x62C8+1);     //    base64
    // hook_native_addr(base_addr, 0x45AC+1);   // static meituan0sankuai1
    // hook_native_addr(base_addr, 0x2984 + 1); 
    // hook_native_addr(base_addr, 0x43A4 + 1); 
    hook_native_addr(base_addr, 0xDDF8 + 1); 
    // hook_native_addr(base_addr, 0xDF10 + 1);  //x
    
    // hook_native_addr(base_addr, 0xCD18 + 1);   //create b64
    // hook_native_addr(base_addr, 0x2F88 + 1); 
    // hook_native_addr(base_addr,  0x36AC + 1);   //
   
    // hook_native_addr(base_addr, 0x3BEE + 1); 
    // hook_native_addr(base_addr,  0x2A70 + 1); 
    // hook_native_addr(base_addr,  0x2DB8 + 1); 

    // hook_native_addr(base_addr,  0x4474 + 1); 
    // hook_native_addr(base_addr,  0x51D8 + 1); 

    // hook_native_addr(base_addr,  0x2B34  + 1);  //hash ?
    // hook_native_addr(base_addrf,  0x36AC  + 1);  //hash ?


    
    
    
    
    
    
    

    
    
    
    // 
    // 45AC
    
    
    
}


var context = null;
function hook_java() {
    Java.perform(function () {
        Java.enumerateClassLoaders({ 
            onMatch: function (loader) {
                try {
                    if (loader.findClass("com.meituan.android.common.datacollection.DataCollectionJni")) {
                        console.log("Successfully found loader")
                        console.log(loader);
                        Java.classFactory.loader = loader;
                    }
                }
                catch (error) {
                    console.log("find error:" + error)
                }
            },
            onComplete: function () {
                console.log("end")
            }
        })
        Java.use("com.meituan.android.common.datacollection.DataCollectionJni").packData.implementation = function(obj, bArr, i) {
            // console.log(obj)
            // if (context==null){
            //     context = Java.retain(context, obj);
            // }
            console.log(Java.use("java.lang.String").$new(bArr))
            // console.log(i)
            var result = this.packData(obj, bArr,i);
            // console.log(Java.use("java.lang.String").$new(result))
            return result


        }
    })
}
function main() {
    hook_java()
    // hook_libart()
}
setImmediate(main)