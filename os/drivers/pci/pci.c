
#include "../../libary/include/stdport.h"
#include "../../libary/include/stdconsole.h"
#include "../../libary/include/util.h"
#include "../include/pci.h"


struct pci_device pci_devices[256]; 
int device_count;



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


void pci_enumerate() {
    device_count = 0;

    for (uint8_t bus = 0; bus < 256; bus++) {
    for (uint8_t dev = 0; dev < 32; dev++) {
    for (uint8_t func = 0; func < 8; func++) {
        uint16_t vendor_id = pci_read_config(bus, dev, func, 0x00) & 0xFFFF;
        if (vendor_id == 0xFFFF) continue; //no device


        uint16_t device_id = (pci_read_config(bus, dev, func, 0x00) >> 16) & 0xFFFF;

              
        struct pci_device* device = &pci_devices[device_count++];
        device->bus = bus;
        device->device = dev;
        device->function = func;
        device->vendor_id = vendor_id;
        device->device_id = device_id;

                
        for (int i = 0; i < 6; i++) {
            device->bar[i] = pci_read_config(bus, dev, func, 0x10 + i * 4);}

             
        device->irq = pci_read_config(bus, dev, func, 0x3C) & 0xFF;


        char buf[12];
        
        print("Pci device"); 
        int_to_str(device_count -1, buf); print(buf); 
        int_to_str(vendor_id, buf); print(buf); 
        int_to_str(device_id, buf); print(buf); 
        int_to_str(device->irq, buf); print(buf); 
    }}}
}
