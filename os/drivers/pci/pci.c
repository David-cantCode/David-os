
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
    for (int bus = 0; bus < 256; bus++) {
    for (int dev = 0; dev < 32; dev++) {
        //read ven id of func 0
        uint16_t vendor_id = pci_read_config((uint8_t)bus, (uint8_t)dev, 0, 0x00) & 0xFFFF;
        if (vendor_id == 0xFFFF) continue; //no device

        //multifunc deck
        uint8_t header_type = (pci_read_config((uint8_t)bus, (uint8_t)dev, 0, 0x0C) >> 16) & 0xFF;
        int function_limit = (header_type & 0x80) ? 8 : 1;


        for (int func = 0; func < function_limit; func++) {
            uint16_t vendor = pci_read_config((uint8_t)bus, (uint8_t)dev, (uint8_t)func, 0x00) & 0xFFFF;
            if (vendor == 0xFFFF) continue;

            uint16_t device_id = (pci_read_config((uint8_t)bus, (uint8_t)dev, (uint8_t)func, 0x00) >> 16) & 0xFFFF;

             
            if (device_count >= (int)(sizeof(pci_devices)/sizeof(pci_devices[0]))) {print("PCI device table full\n");return;}

            struct pci_device* device = &pci_devices[device_count++];
            device->bus = (uint8_t)bus;
            device->device = (uint8_t)dev;
            device->function = (uint8_t)func;
            device->vendor_id = vendor;
            device->device_id = device_id;

            //read BARs
            for (int i = 0; i < 6; i++) {
                device->bar[i] = pci_read_config((uint8_t)bus, (uint8_t)dev, (uint8_t)func, 0x10 + i * 4);}

            //get irq
            device->irq = pci_read_config((uint8_t)bus, (uint8_t)dev, (uint8_t)func, 0x3C) & 0xFF;

               
            char buf[32];
            print("PCI Device ");
            int_to_str(device_count - 1, buf); print(buf);
            print(" Vendor: 0x"); int_to_str(device->vendor_id, buf); print(buf);
            print(" Device: 0x"); int_to_str(device->device_id, buf); print(buf);
            print(" IRQ: "); int_to_str(device->irq, buf); print(buf);
            print("\n");
 

}}}}

struct pci_device* pci_find_device(uint16_t vendor_id, uint16_t device_id) {
    for (int i = 0; i < device_count; i++) {
        if (pci_devices[i].vendor_id == vendor_id &&
            pci_devices[i].device_id == device_id) {
            return &pci_devices[i];
        }
    }
    return 0;
}
