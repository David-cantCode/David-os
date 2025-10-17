#include <stdint.h>
#include "stdport.h"
#include "stdsector.h"

#define ATA_PRIMARY_IO  0x1F0
#define ATA_PRIMARY_CTRL 0x3F6
#define ATA_STATUS      0x1F7
#define ATA_CMD_READ    0x20
#define ATA_STATUS_BSY  0x80
#define ATA_STATUS_RDY  0x40
#define ATA_STATUS_DRQ  0x08


void ata_wait_busy() {
    while (inb(ATA_STATUS) & ATA_STATUS_BSY);
}

void ata_wait_drq() {
    while (!(inb(ATA_STATUS) & ATA_STATUS_DRQ));
}

void read_sector(uint32_t lba, uint8_t* buffer) {
    ata_wait_busy();

    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));   // Drive/head
    outb(0x1F2, 1);                             // Sector count
    outb(0x1F3, (uint8_t)lba);                  // LBA low
    outb(0x1F4, (uint8_t)(lba >> 8));           // LBA mid
    outb(0x1F5, (uint8_t)(lba >> 16));          // LBA high
    outb(0x1F7, ATA_CMD_READ);                  // Command

    ata_wait_busy();
    ata_wait_drq();

    insw(ATA_PRIMARY_IO, buffer, 256); // 256 words = 512 bytes
}
