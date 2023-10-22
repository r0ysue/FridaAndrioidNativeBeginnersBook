// function hooksiua(){
//     Java.perform(function () {
//         var datacollect = Java.use('com.meituan.android.common.datacollection.DataCollectionJni');
//         var ByteString = Java.use("com.android.okhttp.okio.ByteString");
//         datacollect.packData.implementation = function (str1, str2, str3){
//             var result = this.packData(str1, str2, str3);
//             console.log("length:"+str3);
//             console.log("plaintext:"+ByteString.of(str2).utf8())
//             console.log("result:"+ByteString.of(result).utf8());
//             return result;
//         }
//     });
// }

// function callsiua(){
//     Java.perform(function () {
//         var datacollect = Java.use('com.meituan.android.common.datacollection.DataCollectionJni');
//         var ByteString = Java.use("com.android.okhttp.okio.ByteString");
//         var JavaString = Java.use("java.lang.String");
//         var currentApplication= Java.use("android.app.ActivityThread").currentApplication();
//         var context = currentApplication.getApplicationContext();
//         var plainText = "1.0}}sdm845|Xiaomi|polaris|polaris|QKQ1.190828.002|zh|CN|Xiaomi|MIX 2S|10|29|release-keys|Xiaomi/polaris/polaris:10/QKQ1.190828.002/V12.0.2.0.QDGCNXM:user/release-keys|qcom|c3-miui-ota-bd134.bj|user|polaris|arm64-v8a||polaris-user 10 QKQ1.190828.002 V12.0.2.0.QDGCNXM release-keys|1|0|}}adb|mtp,adb|mtp,adb|4.0.c2.6-00335-0914_2350_3c8fca6,4.0.c2.6-00335-0914_2350_3c8fca6|Qualcomm RIL 1.0|wlan0|LOADED,ABSENT||0|0|1|1|1|1|1|1|1|1|1|1}}AArch64 Processor rev 13 (aarch64)|Qualcomm Technologies, Inc SDM845|8|ICM20690|qualcomm|gravity  Non-wakeup|qualcomm}}-|-|-|2030*1080|||02:00:00:00:00:00|46002|4G}}-|0|1|0|0|-|913952e2-16d9-4de7-af87-e7d3bb929ffa}}1|0|0|AQIA}}Android|com.sankuai.meituan|10.0.602|29|-|2021-05-12 09:30:11:011}}0.0|0.0|-|-|0|-|460|00|-|}}"
//         var textLength = plainText.length;
//         var bytesStr = JavaString.$new(plainText).getBytes();
//         var result = datacollect.packData(context, bytesStr, textLength)
//         console.log("result:"+ByteString.of(result).utf8());
//     });
// }

function testsiua(plainText){
    Java.perform(function () {
        var datacollect = Java.use('com.meituan.android.common.datacollection.DataCollectionJni');
        var ByteString = Java.use("com.android.okhttp.okio.ByteString");
        var Base64 = Java.use("java.util.Base64");

        // 参数1是上下文
        var currentApplication= Java.use("android.app.ActivityThread").currentApplication();
        var context = currentApplication.getApplicationContext();
        // 参数2是明文字节数组
        // 使用String类将我们传入的字符串转成数组
        var JavaString = Java.use("java.lang.String");
        var bytesStr = JavaString.$new(plainText).getBytes();
        // 参数3是明文的长度
        var textLength = plainText.length;

        // 主动调用
        var result = datacollect.packData(context, bytesStr, textLength)
        // 返回base64编码后的结果，使用系统类转换成十六进制
        var base64result = ByteString.of(result).utf8()
        var base64before = Base64.getDecoder().decode(base64result);
        // console.log("base64 result:"+base64result);
        console.log("result:"+ByteString.of(base64before).hex());


    });
}
//frida -UF -l C:\Users\pr0214\Desktop\DTA\SO基础课4月\对称加密算法\资料\AES\样本\mt\frida\hookAndcallJava.js