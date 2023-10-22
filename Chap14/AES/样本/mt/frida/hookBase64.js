

function hook_base64(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.arg0 = args[0];
            this.arg1 = args[1];
            this.arg2 = args[2]
            console.log(hexdump(this.arg0));
            console.log("length:"+this.arg1);
            console.log(hexdump(this.arg2));


            // 获得函数的返回地址  64位寄存器是lr
            this.lr = this.context.lr;
        }, onLeave: function (retval) {
            // console.log(JSON.stringify(this.context))
            // console.log("ptr:" + ptr(real_addr) + "idb_addr:" + ptr(offset_addr) + " LR:" + ptr(this.lr).sub(base_addr) + " \r\n",

            // console.log('called from:\n' +
            //     Thread.backtrace(this.context, Backtracer.ACCURATE).map(DebugSymbol.fromAddress).join('\n') + '\n');
        }
    });
}


// 这一步的长度已经padding过了
//  length:0x20
// called from:
// 0xaf4d9ae7 libmtguard.so!0x2ae7
// 0xaf4d9b2d libmtguard.so!0x2b2d
// 0xaf4db4bb libmtguard.so!0x44bb
// 0xaf4db57d libmtguard.so!0x457d
// 0xaf4db8f5 libmtguard.so!0x48f5
// 0xaf4e4e47 libmtguard.so!0xde47
// 0xaf4e3d4d libmtguard.so!0xcd4d
// 0xaf4decf9 libmtguard.so!0x7cf9
// 0xb68c2ec7 base.odex!0x400ec7
//
// length:0x10
// called from:
// 0xaf4d9a23 libmtguard.so!0x2a23
// 0xaf4d997b libmtguard.so!0x297b
// 0xaf4e4e5d libmtguard.so!0xde5d
// 0xaf4e3d4d libmtguard.so!0xcd4d
// 0xaf4decf9 libmtguard.so!0x7cf9
// 0xb68c2ec7 base.odex!0x400ec7
function hook_aes(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.arg0 = args[0];
            this.arg1 = args[1];
            this.arg2 = args[2];
            this.arg3 = args[3];
            this.arg4 = args[4];
            this.arg5 = args[5];

            console.log("length:"+this.arg2);
            // console.log('called from:\n' +
            //     Thread.backtrace(this.context, Backtracer.ACCURATE).map(DebugSymbol.fromAddress).join('\n') + '\n');

            // 参数1
            // 78 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f
            // PCKS # 7
            console.log(hexdump(this.arg0));
            // 参数2 buffer，存放结果
            // console.log(hexdump(this.arg1));

            // 那剩下这个就是密钥
            // 74 69 65 6d 30 6e 61 75 6b 6e 61 73 31 69 61 75  tiem0nauknas1iau
            // 他妈的，从代码里看，应该是aes 256，但数据验证，发现是aes 128的密钥扩展！
            // 密钥扩展部分被魔改了！！
            console.log(hexdump(this.arg3));
            // 存放IV 0102030405060708(str)
            console.log(hexdump(this.arg4));
            // console.log(this.arg5);

        }, onLeave: function (retval) {
            console.log("AES result:\n")
            console.log(hexdump(this.arg1));
            // 68c78bab959028f082a844a60a055d39
        }
    });
}


function hook_expand_keys(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.arg0 = args[0];
            this.arg1 = args[1];
            this.arg2 = args[2]
            console.log(hexdump(this.arg0));
            console.log("length:"+this.arg1);
            console.log(hexdump(this.arg2));


        }, onLeave: function (retval) {
        }
    });
}

function hook_compute_aes(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.arg0 = args[0];
            this.arg1 = args[1];
            this.arg2 = args[2]
            console.log(hexdump(this.arg0));
            console.log(hexdump(this.arg1));
            console.log(hexdump(this.arg2));
        }, onLeave: function (retval) {
        }
    });
}


// 0x3f383f37  第4个明文
// 0x75616931  第四个key倒序
// result:0x4a595606
// 它addroundkeys 是倒序

// 0x3f3a3f39 第3个明文
// 0x73616e6b 第三个key倒序
// 0x4c5b5152
function inline_key_xor(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            console.log(this.context.r0);
            // console.log(this.context.r4);
        }, onLeave: function (retval) {
        }
    });
}

// 还是没有hook到key xor后的地址！
function inline_key_xor_result(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            console.log(hexdump(this.context.sp));
            // console.log(this.context.r3);
        }, onLeave: function (retval) {
        }
    });
}


function hook_4f20(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.arg0 = args[0];
            this.arg1 = args[1];
            console.log(hexdump(this.arg0));
            console.log(this.arg1);
        }, onLeave: function (retval) {
            console.log(hexdump(this.arg0));
            console.log(this.arg1);
        }
    });
}

function hook_29DA(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            console.log(hexdump(this.context.r1));
        }, onLeave: function (retval) {
        }
    });
}

function hook_2984(base_addr, offset_addr) {
    //函数在内存中的地址
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            console.log(hexdump(this.context.r0));
        }, onLeave: function (retval) {
            console.log('called from:\n' +
                Thread.backtrace(this.context, Backtracer.ACCURATE).map(DebugSymbol.fromAddress).join('\n') + '\n');
        }
    });
}

function hook_de5c(base_addr, offset_addr) {
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            console.log(hexdump(this.context.r0));
        }, onLeave: function (retval) {
        }
    });
}

function hook_3e20(base_addr, offset_addr) {
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
        }, onLeave: function (retval) {
            console.log(hexdump(retval));
        }
    });
}

// 可以确定，这就是对明文处理的核心函数
function hook_fca4(base_addr, offset_addr) {
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.args0 = args[0];
            this.args1 = args[1];
            this.args2 = args[2];
            this.args3 = args[3];
            console.log(hexdump(this.args0))
            console.log(hexdump(this.args1))
            console.log(hexdump(this.args2))
            console.log(this.args3)

        }, onLeave: function (retval) {
            console.log(hexdump(this.args0))
        }
    });
}


function hook_fc1c(base_addr, offset_addr) {
    var real_addr = base_addr.add(offset_addr)
    Interceptor.attach(real_addr, {
        onEnter: function (args) {
            this.args0 = args[0];
            this.args1 = args[1];
            this.args2 = args[2];
            this.args3 = args[3];
            this.args4 = args[4];
            console.log(hexdump(this.args0))
            console.log(hexdump(this.args1))
            console.log(hexdump(this.args2))
            console.log(this.args3)
            console.log(this.args4)

        }, onLeave: function (retval) {
            // console.log(hexdump(this.args0))
        }
    });
}

// 0xf4a0a2d5 libc.so!je_free+0x4c
// 0xb8fede5d libmtguard.so!0xde5d
// 0xb8fecd4d libmtguard.so!0xcd4d
// 0xb8fe7cf9 libmtguard.so!0x7cf9
// 0xc3484ec7 base.odex!0x400ec7

function main(){
    var base_addr = Module.findBaseAddress("libmtguard.so");
    // hook_native_addr(base_addr, 0x62C8 + 1)
    // hook_aes(base_addr, 0x36AC + 1)
    // hook_expand_keys(base_addr, 0x2b34 + 1)
    // hook_compute_aes(base_addr, 0x2f88+1)
    // inline_key_xor(base_addr, 0x2FD2+1)
    // inline_key_xor_result(base_addr, 0x2FE2 + 1)
    // hook_4f20(base_addr, 0x4F20 + 1)
    // hook_29DA(base_addr, 0x29DA+1)
    // hook_2984(base_addr, 0x2984+1)
    // hook_de5c(base_addr, 0xde5c+1)
    // hook_3e20(base_addr, 0x3e20+1);
    // hook_fca4(base_addr, 0xfca4+1);
    hook_fc1c(base_addr,0xfc1c+1)
}

// TODO
// 确认aes前的明文在哪儿生成，找到具体的生成函数。

// frida -UF -l C:\Users\pr0214\Desktop\DTA\SO基础课4月\对称加密算法\资料\AES\样本\mt\frida\hookBase64.js