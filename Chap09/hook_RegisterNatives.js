var ENV = null;
var JCLZ = null;

var method01addr = null;
var method02addr = null;
var method02 = null;

var addrNewStringUTF = null;

var NewStringUTF = null;


function hook_RegisterNatives() {
    var symbols = Module.enumerateSymbolsSync("libart.so");
    var addrRegisterNatives = null;
    for (var i = 0; i < symbols.length; i++) {
        var symbol = symbols[i];
        
        //_ZN3art3JNI15RegisterNativesEP7_JNIEnvP7_jclassPK15JNINativeMethodi
        if (symbol.name.indexOf("art") >= 0 &&
                symbol.name.indexOf("JNI") >= 0 && 
                symbol.name.indexOf("NewStringUTF") >= 0 && 
                symbol.name.indexOf("CheckJNI") < 0) {
            addrNewStringUTF = symbol.address;
            console.log("NewStringUTF is at ", symbol.address, symbol.name);
            NewStringUTF = new NativeFunction(addrNewStringUTF,'pointer',['pointer','pointer'])
        }
        if (symbol.name.indexOf("art") >= 0 &&
                symbol.name.indexOf("JNI") >= 0 && 
                symbol.name.indexOf("RegisterNatives") >= 0 && 
                symbol.name.indexOf("CheckJNI") < 0) {
            addrRegisterNatives = symbol.address;
            console.log("RegisterNatives is at ", symbol.address, symbol.name);
        }
    }

    if (addrRegisterNatives != null) {
        Interceptor.attach(addrRegisterNatives, {
            onEnter: function (args) {
                console.log("[RegisterNatives] method_count:", args[3]);
                var env = args[0];
                ENV = args[0];
                var java_class = args[1];
                JCLZ = args[1];
                var class_name = Java.vm.tryGetEnv().getClassName(java_class);
                //console.log(class_name);

                var methods_ptr = ptr(args[2]);

                var method_count = parseInt(args[3]);
                for (var i = 0; i < method_count; i++) {
                    var name_ptr = Memory.readPointer(methods_ptr.add(i * Process.pointerSize * 3));
                    var sig_ptr = Memory.readPointer(methods_ptr.add(i * Process.pointerSize * 3 + Process.pointerSize));
                    var fnPtr_ptr = Memory.readPointer(methods_ptr.add(i * Process.pointerSize * 3 + Process.pointerSize * 2));

                    var name = Memory.readCString(name_ptr);
                    var sig = Memory.readCString(sig_ptr);
                    var find_module = Process.findModuleByAddress(fnPtr_ptr);
                    console.log("[RegisterNatives] java_class:", class_name, "name:", name, "sig:", sig, "fnPtr:", fnPtr_ptr, "module_name:", find_module.name, "module_base:", find_module.base, "offset:", ptr(fnPtr_ptr).sub(find_module.base));
                    if(name.indexOf("method01")>=0){
                        // method01addr = fnPtr_ptr;
                        continue;
                    }else if (name.indexOf("decrypt")>=0){
                        method02addr = fnPtr_ptr;
                        method02 = new NativeFunction(method02addr,'pointer',['pointer','pointer','pointer']);
                        method01addr = Module.findExportByName("libroysue.so", "Java_com_roysue_easyso1_MainActivity_method01")
                    }else{
                        continue;
                    }

                }
            }
        });
    }
}


function invokemethod01(contents){
    
    console.log("method01_addr is =>",method01addr)
    var method01 = new NativeFunction(method01addr,'pointer',['pointer','pointer','pointer']);
    var NewStringUTF = new NativeFunction(addrNewStringUTF,'pointer',['pointer','pointer'])
    var result = null;
    Java.perform(function(){    
        console.log("Java.vm.getEnv()",Java.vm.getEnv())
        var JSTRING = NewStringUTF(Java.vm.getEnv(),Memory.allocUtf8String(contents))
        result = method01(Java.vm.getEnv(),JSTRING,JSTRING);
        console.log("result is =>",result)
        console.log("result is ",Java.vm.getEnv().getStringUtfChars(result, null).readCString())
        result = Java.vm.getEnv().getStringUtfChars(result, null).readCString();

    })
    return result;
}

function invokemethod02(contents){
    var result = null;
    Java.perform(function(){    
        var JSTRING = NewStringUTF(Java.vm.getEnv(),Memory.allocUtf8String(contents))
        result = method02(Java.vm.getEnv(),JSTRING,JSTRING);
        result = Java.vm.getEnv().getStringUtfChars(result, null).readCString();
    })
    return result;
}
rpc.exports = {
    invoke1:invokemethod01,
    invoke2:invokemethod02
};

setImmediate(hook_RegisterNatives);


/*
java_class: com.example.demoso1.MainActivity name: method01 sig: (Ljava/lang/String;)Ljava/lang/String; fnPtr: 0x73e2cd1018 module_name: libnative-lib.so module_base: 0x73e2cc1000 offset: 0x10018
java_class: com.example.demoso1.MainActivity name: method02 sig: (Ljava/lang/String;)Ljava/lang/String; fnPtr: 0x73e2cd0efc module_name: libnative-lib.so module_base: 0x73e2cc1000 offset: 0xfefc

function hookmethod(addr){
    Interceptor.attach(addr,{
        onEnter:function(args){
            console.log("args[0]=>",args[0])
            console.log("args[1]=>",args[1])
            console.log("args[2]=>",Java.vm.getEnv().getStringUtfChars(args[2], null).readCString())
        },onLeave:function(retval){
            console.log(Java.vm.getEnv().getStringUtfChars(retval, null).readCString())
        }
    })
}


function replacehook(addr){
    //> 能够hook上，就能主动调用
    var addrfunc = new NativeFunction(addr,'pointer',['pointer','pointer','pointer']);
    Interceptor.replace(addr,new NativeCallback(function(arg1,arg2,arg3){
        // 确定主动调用可以成功，只要参数合法，地址正确
        var result = addrfunc(arg1,arg2,arg3)
        console.log(arg1,arg2,arg3)
        console.log("result is ",Java.vm.getEnv().getStringUtfChars(result, null).readCString())
        return result;
    },'pointer',['pointer','pointer','pointer']))
}


*/