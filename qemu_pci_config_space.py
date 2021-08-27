import sys

edu_config_space={0x0:0x11e81234, 0x4:0,0x8:0x00ff0010, 0xc:0x00,0x10:0xFFF00000,0x14:0,0x18:0,0x1c:0,0x20:0,0x24:0,0x30:0}
def pci_config_space_read(addr,len):
    print("python:addr is %x, len is %d" % (addr,len))
    byte_offset=addr % 4
    print("python:byte offset is %d" % byte_offset)
    if byte_offset > 0:
        reg_offset = addr/4
        reg_offset *= 4
    else:
        reg_offset = addr
    print("python:reg_offset is 0x%x" % reg_offset)
    mask = 0xFFFFFFFF
    mask = mask >> ((4-len)*8)
    print("python:len mask is 0x%x" % mask)
    value = edu_config_space[reg_offset]
    value = value >> (byte_offset*8)
    value = value & mask
    print("python:value is 0x%x" % value)
    return value

if __name__ == "__main__":
    pci_config_space_read(0,2)
    pci_config_space_read(2,2)
    pci_config_space_read(0xb,1)
    pci_config_space_read(0xa,2)