setImmediate(function () {
    console.log("Invoking...")
    var method01_addr = Module.findExportByName("libroysue.so", "Java_com_roysue_easyso1_MainActivity_method02");
    console.log("method01 addr is  =>", method01_addr)


    Java.perform(function () {
        var jstring = Java.vm.getEnv().newStringUtf("4e8de2f3c674d8157b4862e50954d81c")
        var method01 = new NativeFunction(method01_addr, "pointer", ["pointer", "pointer", "pointer"]);
        var result = method01(Java.vm.getEnv(), jstring, jstring)
        console.log("final result is => ", Java.vm.getEnv().getStringUtfChars(result, null).readCString());
    })
})

// 4e8de2f3c674d8157b4862e50954d81c