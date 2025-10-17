
#ifndef  FAT16_H
#define FAT16_H

#define SECTOR_SIZE       512
#define CLUSTER_SIZE      1      // sectors per cluster
#define FAT_START         1      // FAT starts at sector 1
#define FAT_SECTORS       4
#define ROOT_DIR_START    (FAT_START + FAT_SECTORS)
#define ROOT_DIR_SECTORS  4
#define DATA_START        (ROOT_DIR_START + ROOT_DIR_SECTORS)






#endif