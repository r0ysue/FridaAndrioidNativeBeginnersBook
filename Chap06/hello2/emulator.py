import unicorn 
import capstone
import binascii

# 142a:       4608            mov     r0, r1
# 141c:       4479            add     r1, pc

def testthumb():
    # CODE = b'\x08\x46\x79\x44'
    CODE =  b'\x0a\x46\x03\x46'
    CP = capstone.Cs(capstone.CS_ARCH_ARM,capstone.CS_MODE_THUMB)
    for i in CP.disasm(CODE,0,len(CODE)):
        print("[addr:%x]:%s %s\n"%(i.address,i.mnemonic,i.op_str))
    
    mu=unicorn.Uc(unicorn.UC_ARCH_ARM,unicorn.UC_MODE_THUMB)
    ADDRESS=0x1000
    SIZE=1024
    mu.mem_map(ADDRESS,SIZE)
    mu.mem_write(ADDRESS,CODE)
    bytes = mu.mem_read(ADDRESS,10)
    print("ADDRESS:0x%x,contents:%s"%(ADDRESS,binascii.b2a_hex (bytes)))
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R0,0x100)
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R1,0x200)
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R2,0x300)
    mu.reg_write(unicorn.arm_const.UC_ARM_REG_R3,0x400)
    mu.emu_start(ADDRESS+1,ADDRESS+4)
    print("result is 0x%x"%(mu.reg_read(unicorn.arm_const.UC_ARM_REG_R2)))
    print("result is 0x%x"%(mu.reg_read(unicorn.arm_const.UC_ARM_REG_R3)))


if __name__=='__main__':
    testthumb()