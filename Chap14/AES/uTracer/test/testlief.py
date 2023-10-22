import lief

with open(r"/example/lib/base_md5.so", "rb")as sobinary:
    elf = lief.parse(sobinary)
# TODO: SECTION 被抹掉的情况下，LIEF可以顺利加载relocation吗？不知道它是不是通过section header获取的这些。
for r in elf.relocations:
    if r.symbol.is_function:
        if r.symbol.name == "_ZN7_JNIEnv12NewStringUTFEPKc":
            print(r.symbol.name)

    # if r.symbol.is_function:
    #     # print(r.symbol.name)
    #     if r.symbol.value != 0:
    #         self.mem_write(r.address, r.symbol.value.to_bytes(4,
    #                                                           byteorder='little'))
    #         # 这儿为什么减1呢？
    #         self.functions[r.symbol.value - 1] = r.symbol.name
    #         self.functionsNames.append(r.symbol.name)
    #     else:
    #         self.mem_write(r.address,
    #                        r.address.to_bytes(4, byteorder='little'))
    #         self.functions[r.address] = r.symbol.name
    #         self.functionsNames.append(r.symbol.name)