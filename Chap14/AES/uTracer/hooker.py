# trace and hooker
# 主要实现三部分
# 1.简易的调试器
# 2.精良美观的arm trace
# 3.易用的jni/函数 trace
import subprocess

process_results = subprocess.run("tools\cpp_demangle\cpp_demangle.exe {}".format("_Z8MD5_InitP12MDstate_st1"),
                                         stdout=subprocess.PIPE, stderr=subprocess.PIPE)
print(process_results.stdout.decode("utf-8").strip())