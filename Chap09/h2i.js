function hookJava(){
    Java.perform(function(){
        console.log("hooking java...")
        Java.use("com.roysue.easyso1.MainActivity").decrypt.implementation = function(str){
            var result = this.decrypt(str)
            console.log("str,result => ,",str,result)
            return result;
        }

    })
}
function invokeJava(){
    Java.perform(function(){
        var result  = Java.use("com.roysue.easyso1.MainActivity").decrypt("82e8edd5b05654bf0fedcdfc1c9b4b0f")    
        console.log("result => ,",result)      
        
    })
}


var is_hook_libart = false;

function hook_dlopen() {
    Interceptor.attach(Module.findExportByName(null, "dlopen"), {
        onEnter: function(args) {
            var pathptr = args[0];
            if (pathptr !== undefined && pathptr != null) {
                var path = ptr(pathptr).readCString();
                console.log("dlopen:", path);
                if (path.indexOf("libroysue.so") >= 0) {
                    this.can_hook_libart = true;
                    console.log("[dlopen:]", path);
                }
            }
        },
        onLeave: function(retval) {
            if (this.can_hook_libart && !is_hook_libart) {                
                is_hook_libart = true;
                
            }
        }
    })

    Interceptor.attach(Module.findExportByName(null, "android_dlopen_ext"), {
        onEnter: function(args) {
            var pathptr = args[0];
            if (pathptr !== undefined && pathptr != null) {
                var path = ptr(pathptr).readCString();
                console.log("android_dlopen_ext:", path);
                if (path.indexOf("libroysue.so") >= 0) {
                    this.can_hook_libart = true;
                    console.log("[android_dlopen_ext:]", path);
                }
            }
        },
        onLeave: function(retval) {
            if (this.can_hook_libart && !is_hook_libart) {
                is_hook_libart = true;
                var method01 = Module.findExportByName("libroysue.so", "Java_com_roysue_easyso1_MainActivity_method01")
                var method02 = Module.findExportByName("libroysue.so", "_Z8method02P7_JNIEnvP7_jclassP8_jstring")
                console.log("method01 address is =>",method01)
                console.log("method02 address is =>",method02)
                // Interceptor.attach(method01,{
                //     onEnter:function(args){
                //         console.log("arg[2=>",args[2])

                //     },onLeave:function(retval){
                //         console.log("retval=>",retval)

                //     }
                // })
                // Interceptor.attach(method02,{
                //     onEnter:function(args){
                //         console.log("method02 arg[2=>",Java.vm.getEnv().getStringUtfChars(args[2], null).readCString())

                //     },onLeave:function(retval){
                //         console.log("method02  retval=>",Java.vm.getEnv().getStringUtfChars(retval, null).readCString())
                //     }
                // })
            }
        }
    });
}

function main(){
    console.log("Entering main...")
    // hookJava()
    hook_dlopen()
}
setImmediate(main)