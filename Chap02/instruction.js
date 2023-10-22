function dis(address, number) {
    for(var i = 0; i < number; i++) {
        var ins = Instruction.parse(address);
        console.log("address:" + address + "--dis:" + ins.toString());
        address = ins.next;
    }
}

setImmediate(function(){
    var stringFromJniaddr = Module.findExportByName("libroysue.so","Java_com_roysue_easyso1_MainActivity_stringFromJNI")
    dis(stringFromJniaddr, 10);
})