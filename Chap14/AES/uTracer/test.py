import platform
import subprocess


def name_demangle(name):
    env = platform.system()
    if env == "Windows":
        process_results = subprocess.run("tools/cpp_demangle {}".format(name),
                                         stdout=subprocess.PIPE,
                                         stderr=subprocess.PIPE)
        name_demangle = process_results.stdout.decode("utf-8").strip()
        return name_demangle
    else:
        import cxxfilt
        return cxxfilt.demangle(name)


print("命名粉碎后的符号名：_ZNSt22condition_variable_anyD2Ev")
true_name = name_demangle("_ZNSt22condition_variable_anyD2Ev")
print("还原后："+true_name)

