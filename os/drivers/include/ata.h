#ifndef ATA_H
#define ATA_H
#include <stdint.h>


//registers

#define ata_io 0x1F0 //read/write sect data
#define ata_ctrl 0x3F6


#define ata_data 0x00
#define ata_error 0x01
#define ata_sec_count 0x02 //num of secots to r/w

#define ata_lba0 0x03 //lower 8 lba bits
#define ata_lba1 0x04 // 8-15 
#define ata_lba2 0x05 // 16-23

#define ata_drive_head 0x06 //selects the drive and sends the top 4 bits of lba (24-27)
#define ata_command 0x07 // write the command code 

#define ata_status 0x07 // read what the drive is doing 

#define ata_cmd_read_secs 0x20 

//reading reg inb(0x1F7) gives one of these
#define ata_busy 0x80
#define ata_ready 0x40
#define ata_request 0x08 // drive has data thats ready to transfer
//and error define above (0x01)





void ata_wait();
void read_sector(uint32_t lba, uint8_t* buffer);
void write_sector(uint32_t lba, uint8_t* buffer);

#endif