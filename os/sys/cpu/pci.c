
#include "../../libary/include/stdport.h"
#include "pci.h"

uint32_t pci_config_address(uint8_t bus, uint8_t device,uint8_t function, uint8_t offset) {
    return (1U << 31) | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8)| (offset & 0xFC);    
}


uint32_t pci_read_config(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    outl(0xCF8, pci_config_address(bus, device, function, offset));
    return inl(0xCFC);
}

void pci_write_config(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t value) {
    outl(0xCF8, pci_config_address(bus, device, function, offset));
    outl(0xCFC, value);
}
