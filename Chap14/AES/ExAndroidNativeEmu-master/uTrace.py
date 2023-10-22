import hexdump
from unicorn import UC_ARCH_ARM, UC_HOOK_CODE, UC_QUERY_MODE, arm_const, \
    UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE, UC_MEM_WRITE, UC_HOOK_MEM_INVALID, \
    UC_MEM_READ_UNMAPPED, UC_MEM_FETCH_PROT, UC_MEM_READ_PROT, \
    UC_MEM_WRITE_UNMAPPED, UC_MEM_WRITE_PROT, UC_MEM_FETCH_UNMAPPED

import capstone as cs

# pygments，格式化显示和输出，突出一个优雅
from pygments import highlight
from pygments.formatters.terminal import TerminalFormatter as formatter
from pygments.lexers.asm import NasmLexer
# 给输出增添颜色
import colorama

'''
一个unicorn trace 和 debug 插件，暂时只支持arm32。
代码大部分剪裁自Rainbow， https://github.com/Ledger-Donjon/rainbow
'''


class UnicornDebugger:
    def __init__(self, mu, baseAddr = 0):

        self._mu = mu

        self.arch = mu._arch
        self.mode = mu._mode

        self.disasm = cs.Cs(cs.CS_ARCH_ARM, self.mode)
        self.disasm.detail = True

        self.breakpoints = []
        self.memory_breakpoints = []

        self.reg_leak = None

        self.asm_hl = NasmLexer()
        self.asm_fmt = formatter(outencoding="utf-8")
        colorama.init()

        # 寄存器们
        self.INTERNAL_REGS = ["r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                              "pc", "lr"]
        known_regs = [i[len('UC_ARM_REG_'):] for i in dir(arm_const) if
                      '_REG' in i]
        self.reg_map = {r.lower(): getattr(arm_const, 'UC_ARM_REG_' + r)
                        for r in known_regs}

        self.baseAddr = baseAddr

        # 目前只支持arm32，其余架构不支持
        if self.arch != UC_ARCH_ARM:
            mu.emu_stop()
            raise NotImplementedError("arch:%d is not supported! " % self.arch)

        # 逐条指令trace
        mu.hook_add(UC_HOOK_CODE, self.code_trace)
        # 内存读写trace
        # mu.hook_add(UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, self.trace_mem)
        # 内存异常的hook
        mu.hook_add(UC_HOOK_MEM_INVALID, self.hook_mem_invalid)
        # TODO
        # hook 基本块，这部分数据由用户输入，即一个map或者文件格式，address+函数名
        # 返回对特定地址的函数标识以及demangle后的函数名

    def code_trace(self, mu, address, size, data):
        # unicorn封装了查询当前是thumb or arm 的方法，其基于pc地址的奇偶。
        self.mode = mu.query(UC_QUERY_MODE)
        self.disasm.mode = self.mode

        # 我们希望打印执行的指令，同时附带上发生改变的寄存器以及其值。
        # 使用capstone完成反汇编，使用unicorn read reg即可完成对寄存器的访问。
        # 存在两个小问题
        # 1.如何只打印"发生改变的寄存器",一种朴素的方法就是保存每条指令执行前后所有寄存器的值，进行对比，打印改变的寄存器值。
        # 但存在一个问题，指令可能对寄存器产生了操作，但寄存器的值，恰好未发生改变。这会给分析带来误导。
        # 多亏了captone的regs_access api， 它可以分析汇编代码，检测出显式或者隐式的对寄存器的读/写，在这里我们只关注写操作。（这个api有bug，需要修复）
        # 2。hook 的时机
        # 示例汇编如下，初始化r0 = 0 。
        '''
        mov r0,  # 0x37
        '''
        # 发生改变的寄存器为r0，我们期待得到的结果如下：
        '''
        mov r0,  # 0x37 ； r0 = 0x37
        '''
        # 但HOOK_CODE的结果会是如下：
        '''
        mov r0,  # 0x37 ； r0 = 0
        '''
        # 这是因为 unicorn hook code 的时机为指令执行前，这样的结果显然是不行的
        # 因此保存当前指令所显式或者隐式修改的寄存器，在下一条指令的hook code回调后打印，避免了错位。

        # ins = self.disassemble_single(address, size)
        # This includes all the implicit & explicit registers.
        # 只关注对寄存器的“写”操作

        # regs_read, regs_written = ins.regs_access()
        # if len(regs_written) > 0:
        #     self.reg_leak = (
        #         address, list(map(ins.reg_name, regs_written)))
        # else:
        #     self.reg_leak = None
        #
        # op_bytes = bytes(ins.bytes).hex()
        # # print asm
        # self.print_asmline(address, self.mode, op_bytes, ins.mnemonic, ins.op_str)
        #
        # if self.reg_leak is not None:
        #     for x in self.reg_leak[1]:
        #         print(f" {x} = {self._mu.reg_read(self.reg_map[x]):08x} ", end=" ")

        if address in self.breakpoints:
            if self.baseAddr:
                print(f"\n********* Breakpoint hit at {address - self.baseAddr:08X}  绝对地址：{address:08X}  *********")
            else:
                print(f"\n********* Breakpoint hit at {address:08X}  *********")

            self.dump_reg()

            while True:
                s = input("Press Enter to continue：")

                if s == '':
                    break
                try:
                    self.self_dbg(s)

                except Exception as e:
                    print("Error:", e)
                    continue

        instruction = self.readPointer(address)
        # if instruction == "0x0" :
        #     mu.emu_stop()
        #     error = "你走到了没有汇编的荒原!"
        #     raise Exception(error)

    def trace_mem(self, uci, access, address, size, value, user_data):
        """ Hook that shows a visual trace of memory accesses in the form '[address written to] <- value written' or 'value read <- [address read]' """
        addr = self.color("BLUE", f"{address:08X}")
        # print(addr)
        if access == UC_MEM_WRITE:
            val = self.color("CYAN", f"{value:x}")
            print(f"  [{addr}] <- {val} ", end=" ")
            if address in self.memory_breakpoints:
                print("{:30}  ---=== {} ===---".format(" ", "WRITE"))
                data = uci.mem_read(address, 0x100)
                print(self.advance_dump(data, address))
        else:
            val = int.from_bytes(uci.mem_read(address, size), "little")
            val = self.color("CYAN", f"{val:8x}")
            print(f"  {val} <- [{addr}]", end=" ")
            if address in self.memory_breakpoints:
                print("{:30}  ---=== {} ===---".format(" ", "READ"))
                data = uci.mem_read(address, 0x100)
                print(self.advance_dump(data, address))

    def hook_mem_invalid(self, uc, access, address, size, value, user_data):
        access_memory_str = ""
        if access == UC_MEM_READ_UNMAPPED:
            access_memory_str = "UC_MEM_READ_UNMAPPED"
        elif access == UC_MEM_WRITE_UNMAPPED:
            access_memory_str = "UC_MEM_WRITE_UNMAPPED"
        elif access == UC_MEM_FETCH_UNMAPPED:
            access_memory_str = "UC_MEM_FETCH_UNMAPPED"
        elif access == UC_MEM_FETCH_PROT:
            access_memory_str = "UC_MEM_FETCH_PROT"
        elif access == UC_MEM_WRITE_PROT:
            access_memory_str = "UC_MEM_WRITE_PROT"
        elif access == UC_MEM_READ_PROT:
            access_memory_str = "UC_MEM_READ_PROT"

        error = f"\n\n{access_memory_str} at {hex(address)}(Emu stopped in {uc.reg_read(self.reg_map['pc']):X})"
        print(error)
        uc.emu_stop()
        return False

    def add_bkpt(self, address):
        if self.baseAddr:
            address += self.baseAddr
        if address not in self.breakpoints:
            self.breakpoints += [address]

    # print all reg
    def dump_reg(self):
        """ Dumps all regs when a breakpoint is hit """
        for reg in self.INTERNAL_REGS:
            print(f"{reg} : {self._mu.reg_read(self.reg_map[reg]):X}")

    # 命令行调试器
    def self_dbg(self, s):
        command = s.split(" ")[0]
        if command in ["hexdump","h","hex"] :
            address, size = s.split(" ")[1:]
            self.dumphex(address, size)
        elif command in ["readPointer", "ptr"]:
            address = eval(s.split(" ")[1])
            print(self.readPointer(address))
        elif command in ["readCString", "str"]:
            address = eval(s.split(" ")[1])
            print(self.readCString(address))
        elif command in ["setreg"]:
            regName, value = s.split(" ")[1:]
            regValue = eval(value)
            self._mu.reg_write(self.reg_map[regName], regValue)
        else:
            print("请检查拼写，当前只支持如下指令")
            print("hexdump address size : hexdump 0x100 0x20")
            print("readPointer address : readPointer 0x1000")
            print("readCString address : readCString 0xC24")
            print("setreg r0 0x100")


    # 反汇编
    def disassemble_single(self, addr, size):
        """ Disassemble a single instruction at addr """
        instruction = self._mu.mem_read(addr, 2 * size)
        return next(self.disasm.disasm(bytes(instruction), addr, 1))

    # show part
    def print_asmline(self,adr,mode,op_bytes, ins, op_str):
        """ Pretty-print assembly using pygments syntax highlighting """
        MODE = "T" if mode else "A"
        line = (
            highlight(f"{ins:<6} {op_str:<20}", self.asm_hl, self.asm_fmt)
                .decode()
                .strip("\n")
        )
        if len(op_bytes) == 4:
            op_bytes = f"  {int(op_bytes, 16):04X}      "
        else:
            op_bytes = f"  {int(op_bytes, 16):08X}  "

        if self.baseAddr:
            print("\n" + MODE + self.color("YELLOW", f" {adr - self.baseAddr:08X} {adr:08X}") + self.color("RED", op_bytes) + line, end=";")
        else:
            print("\n" + MODE + self.color("YELLOW",f" {adr:08X}") + self.color("RED", op_bytes) + line, end=";")

    # 默认hexdump方法索引总是从0开始。
    def advance_dump(self, data, base):
        generator = hexdump.genchunks(data, 16)
        retstr = ''
        for addr, d in enumerate(generator):
            # 使展示的索引从base开始
            line = '%08X: ' % (base + addr * 16)
            dumpstr = hexdump.dump(d)
            line += dumpstr[:8 * 3]
            if len(d) > 8:  # insert separator if needed
                line += ' ' + dumpstr[8 * 3:]
            # ................
            # calculate indentation, which may be different for the last line
            pad = 2
            if len(d) < 16:
                pad += 3 * (16 - len(d))
            if len(d) <= 8:
                pad += 1
            line += ' ' * pad

            for byte in d:
                # printable ASCII range 0x20 to 0x7E
                if 0x20 <= byte <= 0x7E:
                    line += chr(byte)
                else:
                    line += '.'
            retstr += line + '\n'
        return retstr

    def dumphex(self, addr, size):
        print(addr)
        print(size)
        if isinstance(addr,str):
            addr = eval(addr)
        if isinstance(size,str):
            size = eval(size)
        data = bytes(self._mu.mem_read(addr, size))
        ret = self.advance_dump(data, addr)
        print(ret)

    def readPointer(self, address):
        return hex(int.from_bytes(self._mu.mem_read(address, 4), byteorder='little'))

    def readCString(self, address):
        buffer_address = address
        buffer_read_size = 0x10
        buffer = b""
        null_pos = None

        # Keep reading until we read something that contains a null terminator.
        while null_pos is None:
            buf_read = self._mu.mem_read(buffer_address, buffer_read_size)
            if b'\x00' in buf_read:
                null_pos = len(buffer) + buf_read.index(b'\x00')
            buffer += buf_read
            buffer_address += buffer_read_size
        return buffer[:null_pos].decode("utf-8")

    def color(self, name, x):
        return f"{getattr(colorama.Fore, name)}{x}{colorama.Fore.RESET}"

