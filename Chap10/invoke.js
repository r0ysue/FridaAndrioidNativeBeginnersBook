setTimeout(function () {
    Java.perform(function () {
        // Java.use("android.os.Build").FINGERPRINT.value = "12345"

        console.log("found strcat address => ", Module.findExportByName(null, "strcat"));
        var strcat = new NativeFunction(Module.findExportByName(null, "strcat"), "pointer", ["pointer", "pointer"])
        Interceptor.replace(Module.findExportByName(null, "strcat"), new NativeCallback(function (arg0, arg1) {
            console.log("arg[0] is => ", arg0.readCString());
            console.log("arg[1] is => ", arg1.readCString());
            // if(arg1.readCString().indexOf("FAKE")>=0){
            //     var REAL = Memory.allocUtf8String("REAL");
            //     arg1 = REAL ;
            //     console.log("change FAKE to REAL")
            // }
            var retval = strcat(arg0, arg1);
            return retval;
        }, "pointer", ["pointer", "pointer"]));
    })
    // var strcat_addr = Module.findExportByName(null,"strcat")
    // console.log("found address is => ",strcat_addr);
    // var strcat = new NativeFunction(strcat_addr,"pointer",["pointer","pointer"]);
    // Interceptor.replace(strcat_addr, new NativeCallback(function(arg1, arg2){
    //     console.log("arg1 is => ",arg1);
    //     console.log("arg2 is => ",arg2);
    //     var result = strcat(arg1,arg2);
    //     console.log("result is => ",result);
    //   }, "pointer", ["pointer", "pointer"]));

}, 0);

function invoke() {
    Java.perform(function () {


        var result = Java.use("com.roysue.easyso1.MainActivity").Sign("requestUserInfo");
        console.log("result is => ", result);

    })

}