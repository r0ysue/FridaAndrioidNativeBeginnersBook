
function monitor_constants(targetSo) {
    let const_array = [];
    let const_name = [];
    let const_addr = [['padding used in hashing algorithms (0x80 0 ... 0)', '0x3f008'], ['padding used in hashing algorithms (0x80 0 ... 0)', '0x3f048'], ['SHA256 / SHA224 K tabke', '0x31fa8'], ['SHA512 / SHA384 K table', '0x32108']];

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
    const funcs = [['MD5Init', '函数MD5Init疑似哈希函数，包含初始化魔数的代码。', '0x117dc'], ['sub_11A38', '函数sub_11A38疑似哈希函数运算部分。', '0x11a38'], ['SHA_Init', '函数SHA_Init疑似哈希函数，包含初始化魔数的代码。', '0x1332c'], ['sub_1387C', '函数sub_1387C疑似哈希函数运算部分。', '0x1387c'], ['sha256_init', '函数sha256_init疑似哈希函数，包含初始化魔数的代码。', '0x178d4'], ['SHA512Init', '函数SHA512Init疑似哈希函数，包含初始化魔数的代码。', '0x182cc'], ['sub_1C620', '函数sub_1C620疑似哈希函数，包含初始化魔数的代码。', '0x1c620'], ['sub_1D168', '函数sub_1D168疑似哈希函数，包含初始化魔数的代码。', '0x1d168'], ['sub_1E7E8', '函数sub_1E7E8疑似哈希函数，包含初始化魔数的代码。', '0x1e7e8'], ['sub_206F0', '函数sub_206F0疑似哈希函数，包含初始化魔数的代码。', '0x206f0'], ['sub_214EC', '函数sub_214EC疑似哈希函数，包含初始化魔数的代码。', '0x214ec'], ['sub_231C4', '函数sub_231C4疑似哈希函数，包含初始化魔数的代码。', '0x231c4'], ['sub_24BF8', '函数sub_24BF8疑似哈希函数，包含初始化魔数的代码。', '0x24bf8'], ['sub_25224', '函数sub_25224疑似哈希函数，包含初始化魔数的代码。', '0x25224'], ['sub_25D24', '函数sub_25D24疑似哈希函数，包含初始化魔数的代码。', '0x25d24'], ['sub_27030', '函数sub_27030疑似哈希函数，包含初始化魔数的代码。', '0x27030'], ['sub_2ADEC', '函数sub_2ADEC疑似哈希函数，包含初始化魔数的代码。', '0x2adec']];
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
    // 2.ARM 64无法使用
    // 3.无法查看调用栈
    // 在这儿用于验证这些常量是否被访问，访问了就说明可能使用该哈希算法。
    // MemoryAccessMonitor在别处可能有较大用处，比如ollvm过的so，或者ida xref失效/过多等情况。
    // hook和monitor这两个函数，只能分别注入和测试，两个同时会出错，这可能涉及到frida inline hook的原理
    // 除非hook_suspected_function 没结果，否则不建议使用monitor_constants。
    // monitor_constants(targetSo);

    hook_suspected_function(targetSo);
}

setImmediate(main);
    