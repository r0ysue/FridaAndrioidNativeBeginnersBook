function padding(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.arg0 = args[0];
            this.arg1 = args[1];
            console.log("处理后的明文：\n")
            console.log(hexdump(this.arg0));
            // console.log(this.arg1);
        }, onLeave: function (retval) {
            // console.log(hexdump(this.arg0));
            // console.log(this.arg1);
        }
    });
}

// 输入 0
// 78 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f  x...............
// 输入 01
// 78 9c 0e 0e 0e 0e 0e 0e 0e 0e 0e 0e 0e 0e 0e 0e  x...............
// 输入 012
// 78 9c 33 0d 0d 0d 0d 0d 0d 0d 0d 0d 0d 0d 0d 0d  x.3.............
// ......
// 输入 0123456789abcdef
// 78 9c 33 30 34 32 36 31 35 33 b7 b0 4c 4c 4a 4e  x.30426153..LLJN
// 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10  ................
// 输入 0123456789abcdef0123
// 78 9c 33 30 34 32 36 31 35 33 b7 b0 4c 4c 4a 4e  x.30426153..LLJN
// 49 4d 33 00 0c 0c 0c 0c 0c 0c 0c 0c 0c 0c 0c 0c  IM3.............
// 输入 0123456789abcdef0123456789abcdef012301230123456780123456
// 78 9c 33 30 34 32 36 31 35 33 b7 b0 4c 4c 4a 4e  x.30426153..LLJN
// 49 4d 33 c0 c2 87 61 b0 38 94 01 00 92 55 0d 8a  IM3...a.8....U..
// 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10
// 即并非逐位异或
// 这个例子 更明显
// 输入 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
// 78 9c 33 34 1c 20 00 00 85 89 17 2a 04 04 04 04

function hook_keyFunction(base_addr) {
    var real_addr = base_addr.add(0xFC1C + 1)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.args0 = args[0];
            this.args1 = args[1];
            this.args2 = args[2];
            this.args3 = args[3];
            this.args4 = args[4];
            // console.log(hexdump(this.args0))
            console.log("another length:\n")
            console.log(hexdump(this.args1))
            console.log("plainText:\n")
            console.log(hexdump(this.args2))
            console.log("length:\n")
            console.log(this.args3)
            // console.log(this.args4)

        }, onLeave: function (retval) {
            console.log("Fuck plainText:\n")
            console.log(hexdump(this.args0))
        }
    });
}



function main(){
    var base_addr = Module.findBaseAddress("libmtguard.so");
    padding(base_addr, 0x4F20 + 1)
    hook_keyFunction(base_addr);
}

main();

// frida -UF -l C:\Users\pr0214\Desktop\DTA\SO基础课4月\对称加密算法\资料\AES\样本\mt\frida\hookSource.js