function hookstrcmp(){
    Java.perform(function() {

        var addr_strcmp = Module.findExportByName("libc.so","strcmp");
        Interceptor.attach(addr_strcmp, {
            onEnter: function (args) {

                    if(ptr(args[1]).readCString().indexOf("REJECT")>=0){
                        console.log("[*] strcmp (" + ptr(args[0]).readCString() + "," + ptr(args[1]).readCString()+")");
                        this.isREJECT = true;
                    }
            },onLeave:function(retval){
                if(this.isREJECT){
                    retval.replace(0x1);
                    console.log("the REJECT's result :",retval);
                }
            }
        });
    })
}
setImmediate(hookstrcmp);