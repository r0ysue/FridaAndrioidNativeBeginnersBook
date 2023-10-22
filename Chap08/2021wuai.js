function inline_hook() {

    Java.perform(function() {

        var addr_libnative = Module.findBaseAddress("libnative-lib.so");
        
        if (addr_libnative) {
            console.log("so基址: ", addr_libnative);
            var sub_B90 = addr_libnative.add(0xB90);
            Interceptor.attach(sub_B90, {
                onEnter: function(args) {
                    this.arg0 = args[0];
                    this.arg1 = args[1];
                    this.arg2 = args[2];


                    console.log("\n",hexdump(this.arg0));
                    console.log("\n",this.arg1);
                    console.log("\n",hexdump(this.arg2));
                },
                onLeave:function(retval){
                    
                    console.log("-------sub_B90 retval -------");
                    console.log("\n",hexdump(this.arg0));
                    console.log("\n",this.arg1);
                    console.log("\n",hexdump(this.arg2));
                }
            });
        }
    });
}

setImmediate(inline_hook);