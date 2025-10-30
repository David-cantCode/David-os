#ifndef FAT16_H
#define FAT16_H


#include <stdint.h>
#define SECTOR_SIZE 512
#define RESERVED_SECTORS 24 //includes kernel. increase when kernel sectors get increased in boot.asn (+2 of bootloader kernel secs)
#define FAT_COUNT 2
#define SECTORS_PER_FAT 64
#define ROOT_DIR_ENTRIES 512
#define SECTORS_PER_CLUSTER 4

#define TOTAL_SECTORS 65536    
#define FIRST_FAT_SECTOR RESERVED_SECTORS
#define FIRST_ROOT_DIR_SECTOR (FIRST_FAT_SECTOR + FAT_COUNT * SECTORS_PER_FAT)
#define ROOT_DIR_SECTORS ((ROOT_DIR_ENTRIES * 32 + SECTOR_SIZE - 1)/SECTOR_SIZE)
#define FIRST_DATA_SECTOR (FIRST_ROOT_DIR_SECTOR + ROOT_DIR_SECTORS)

#define CLUSTER_COUNT ((TOTAL_SECTORS - FIRST_DATA_SECTOR) / SECTORS_PER_CLUSTER)



void fat16_init();
uint32_t find_safe_cluster();

void create_directory(const char* name, uint16_t parent_cluster, int is_root_parent);
void list_directory(uint16_t dir_cluster);
uint16_t find_directory_cluster(const char* name, uint16_t parent_cluster) ;
char* conv_fat_name(const char* input);
#endif