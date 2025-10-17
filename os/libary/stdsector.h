
#ifdef STDSECTOR_H
#define STDSECTOR_H

void read_sector(uint32_t lba, uint8_t* buffer);
void ata_wait_drq() ;

#endif