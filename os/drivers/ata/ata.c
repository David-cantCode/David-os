
//**********************************************
// ******HARD WARE INTERFACE DRIVER******** 
//********************************** **************

#include "../include/ata.h"
#include "../../libary/include/stdport.h"


//lba - logical block address
// essentially the sector number on the disk.

void ata_wait() {
    //helper func to make sure drive is not busy
    while (inb(ata_io + ata_status) & ata_busy);
}


//reads 512 bytes and puts it into buffer
void read_sector(uint32_t lba, uint8_t* buffer) {
    ata_wait();
  
    outb(ata_io + ata_drive_head, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ata_io + ata_sec_count, 1);
    outb(ata_io + ata_lba0, (uint8_t)lba);
    outb(ata_io + ata_lba1, (uint8_t)(lba >> 8));
    outb(ata_io + ata_lba2, (uint8_t)(lba >> 16));
    outb(ata_io + ata_command, ata_cmd_read_secs);

    ata_wait();

    // pause till request
    while (!(inb(ata_io + ata_status) & ata_request));

    // read 256 words - 512 bytes
    insw(ata_io + ata_data, buffer, 256);
}


void write_sector(uint32_t lba, uint8_t* buffer) {
    ata_wait();

    outb(ata_io + ata_drive_head, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ata_io + ata_sec_count, 1);
    outb(ata_io + ata_lba0, (uint8_t)lba);
    outb(ata_io + ata_lba1, (uint8_t)(lba >> 8));
    outb(ata_io + ata_lba2, (uint8_t)(lba >> 16));
    outb(ata_io + ata_command, 0x30); 

    ata_wait();
    //puase for req
    while (!(inb(ata_io + ata_status) & ata_request));

    //write 512 bytes
    outsw(ata_io + ata_data, buffer, 256);

    ata_wait();
}
