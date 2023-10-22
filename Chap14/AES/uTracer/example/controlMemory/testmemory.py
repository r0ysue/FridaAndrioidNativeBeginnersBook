from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM

emu = Uc(UC_ARCH_ARM, UC_MODE_ARM)
emu.mem_map(0,0x1000)
emu.mem_write(0, bytes([1 for i in range(0x1000)]))
emu.mem_write(0,b"12")
print(bytes(emu.mem_read(0,0x1000)))