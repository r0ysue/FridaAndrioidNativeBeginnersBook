
function monitor_constants(targetSo) {
    let const_array = [];
    let const_name = [];
    let const_addr = [['padding used in hashing algorithms (0x80 0 ... 0)', '0x21000'], ['padding used in hashing algorithms (0x80 0 ... 0)', '0x21040'], ['SHA256 / SHA224 K tabke', '0x1ca2c'], ['SHA512 / SHA384 K table', '0x1cb30']];

    for (var i = 0; i < const_addr.length; i++) {
        const_array.push({base:targetSo.add(const_addr[i][1]),size:0x1});
        const_name.push(const_addr[i][0]);
    }

    MemoryAccessMonitor.enable(const_array, {
        onAccess: function (details) {
            console.log("\n");
            console.log("监控到疑似加密常量的内存访问\n");
            console.log(const_name[details.rangeIndex]);
            console.log("访问来自:"+details.from.sub(targetSo)+"(可能有误差)");
    }
});
}

function hook_suspected_function(targetSo) {
    const funcs = [['MD5Init', '函数MD5Init疑似哈希函数，包含初始化魔数的代码。', '0xa481'], ['sub_A5B0', '函数sub_A5B0疑似哈希函数运算部分。', '0xa5b1'], ['SHA_Init', '函数SHA_Init疑似哈希函数，包含初始化魔数的代码。', '0xb3d5'], ['sub_B598', '函数sub_B598疑似哈希函数运算部分。', '0xb599'], ['sha256_init', '函数sha256_init疑似哈希函数，包含初始化魔数的代码。', '0xd5ab'], ['SHA512Init', '函数SHA512Init疑似哈希函数，包含初始化魔数的代码。', '0xdb49'], ['sub_109D0', '函数sub_109D0疑似哈希函数，包含初始化魔数的代码。', '0x109d1'], ['sub_10FB0', '函数sub_10FB0疑似哈希函数，包含初始化魔数的代码。', '0x10fb1'], ['sub_127C8', '函数sub_127C8疑似哈希函数，包含初始化魔数的代码。', '0x127c9'], ['sub_12C30', '函数sub_12C30疑似哈希函数，包含初始化魔数的代码。', '0x12c31']];
    for (var i in funcs) {
        let relativePtr = funcs[i][2];
        let funcPtr = targetSo.add(relativePtr);
        let describe = funcs[i][1];
        let handler = (function() {
        return function(args) {
            console.log("\n");
            console.log(describe);
            console.log(Thread.backtrace(this.context,Backtracer.ACCURATE).map(DebugSymbol.fromAddress).join("\n"));
        };
        })();
    Interceptor.attach(funcPtr, {onEnter: handler});
}
}


function main() {
    var targetSo = Module.findBaseAddress('libnative-lib.so');
    // 对疑似哈希算法常量的地址进行监控，使用frida MemoryAccessMonitor API，有几个缺陷，在这里比较鸡肋。
    // 1.只监控第一次访问，所以如果此区域被多次访问，后续访问无法获取。可以根据这篇文章做改良和扩展。https://bbs.pediy.com/thread-262104-1.htm
    // 2.ARM 64 SO 无法使用
    // 3.无法查看调用栈
    // 在这儿用于验证这些常量是否被访问，访问了就说明可能使用该哈希算法。
    // MemoryAccessMonitor在别处可能有较大用处，比如ollvm过的so，或者ida xref失效/过多等情况。
    // hook和monitor这两个函数，只能分别注入和测试，两个同时会出错，这可能涉及到frida inline hook的原理
    // 除非hook_suspected_function 没结果，否则不建议使用monitor_constants。
    monitor_constants(targetSo);

    // hook_suspected_function(targetSo);
}

setImmediate(main);
    