setImmediate(function(){
    Java.perform(function(){
        Java.use("com.r0ysue.easyso1.MainActivity").onCreate.implementation = function(x){
            console.log("Entering onCreate!");
            return this.onCreate(x);
        }
        Java.use("com.r0ysue.easyso1.MainActivity").stringFromJNI.implementation = function(){
            var result = this.stringFromJNI();
            console.log("Return value of stringFromJNI is => ", result);
            return result;
        }
    })
})