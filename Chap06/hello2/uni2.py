import unicorn 
import capstone
import binascii

def printArmRegs(mu):
    for i in range(66,78):
        print("R%d,value:%x"%(i-66,mu.reg_read(i)))

def testthumb():
    CODE = b'\x0a\x46\x03\x46'
    CP = capstone.Cs(capstone.CS_ARCH_ARM,capstone.CS_MODE_THUMB)
    for i in CP.disasm(CODE,0,len(CODE)):
        print("[addr:%x]:%s %s\n"%(i.address,i.mnemonic,i.op_str))
    mu=unicorn.Uc(unicorn.UC_ARCH_ARM,unicorn.UC_MODE_THUMB)
    ADDRESS = 0x1000
    SIZE = 1024
    mu.mem_map(ADDRESS,SIZE)
    mu.mem_write(ADDRESS,CODE)
    bytes=mu.mem_read(ADDRESS,10)
    print("ADDRESS:0x%x,contents:%s"%(ADDRESS,binascii.b2a_hex(bytes)))
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R0,0x100)
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R1,0x200)
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R2,0x300)
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R3,0x400)
    printArmRegs(mu)
    mu.emu_start(ADDRESS+1,ADDRESS+4)
    printArmRegs(mu)


if __name__=='__main__':
    testthumb()