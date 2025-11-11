#include <stdint.h>
#include "../include/fat16.h"
#include "../include/util.h"
#include "../../drivers/include/ata.h"
#include "../include/stdconsole.h"
#include "../../apps/terminal/terminal.h"
uint8_t sector_buf[SECTOR_SIZE];




static int find_free_root_slot(uint32_t *out_sector_lba, int *out_offset_in_sector) {
    //-1 not found - if found
    for (int s = 0; s < ROOT_DIR_SECTORS; s++) {
        uint32_t lba = FIRST_ROOT_DIR_SECTOR + s;
        read_sector(lba, sector_buf);
        for (int off = 0; off < SECTOR_SIZE; off += 32) {
            uint8_t first_byte = sector_buf[off];
            if (first_byte == 0x00 || first_byte == 0xE5) {
                *out_sector_lba = lba;
                *out_offset_in_sector = off;
                return 0; 
            }
        }
    }
    return 0; 
}


static uint8_t fatbuf[SECTORS_PER_FAT * SECTOR_SIZE];

void fat_read_full() {
    for (int i = 0; i < SECTORS_PER_FAT; i++) {
        read_sector(FIRST_FAT_SECTOR + i, fatbuf + i * SECTOR_SIZE);
    }
}

static void fat_write_full_to_both() {
    for (int copy = 0; copy < FAT_COUNT; copy++) {
        uint32_t fat_start = FIRST_FAT_SECTOR + copy * SECTORS_PER_FAT;
        for (int i = 0; i < SECTORS_PER_FAT; i++) {
            write_sector(fat_start + i, fatbuf + i * SECTOR_SIZE);
        }
    }
}


void init_fat(uint8_t* fat) {
    memoryset(fat, 0, SECTORS_PER_FAT * SECTOR_SIZE);

    // reseved entries basically tells the disc the disc type and that these areas cant be used for data
    fat[0] = 0xF8; 
    fat[1] = 0xFF;
    fat[2] = 0xFF;
}



void write_fats() {
    uint8_t fat[SECTORS_PER_FAT * SECTOR_SIZE];
    init_fat(fat);

    for (int copy = 0; copy < FAT_COUNT; copy++) {
        for (int i = 0; i < SECTORS_PER_FAT; i++) {
            write_sector(FIRST_FAT_SECTOR + copy*SECTORS_PER_FAT + i, fat + i*SECTOR_SIZE);
        }
    }
}


void init_root_dir() {
    memoryset(sector_buf, 0, SECTOR_SIZE);
    for (int i = 0; i < ROOT_DIR_SECTORS; i++) {
        write_sector(FIRST_ROOT_DIR_SECTOR + i, sector_buf);
    }
}


void write_test_file() {
    memoryset(sector_buf, 0, SECTOR_SIZE);
    memorycpy(sector_buf, "TEST    ", 8);
    memorycpy(sector_buf + 8, "TXT", 3);       
    sector_buf[11] = 0x20;
    sector_buf[26] = 2;
    sector_buf[28] = 512;
    
    
    write_sector(FIRST_ROOT_DIR_SECTOR, sector_buf);
    for (int i = 0; i < SECTOR_SIZE; i++) sector_buf[i] = 0xAA;

    //data n data
    uint32_t lba = FIRST_DATA_SECTOR + (2 - 2) * SECTORS_PER_CLUSTER; 
    for (int i = 0; i < SECTORS_PER_CLUSTER; i++) {
        write_sector(lba + i, sector_buf);
    }

    //  mark cluster as eof (0xFFFF)
    uint8_t fat[SECTORS_PER_FAT * SECTOR_SIZE];
    read_sector(FIRST_FAT_SECTOR, fat); 
    fat[2*2] = 0xFF; 
    fat[2*2+1] = 0xFF; 
    write_sector(FIRST_FAT_SECTOR, fat);
}



uint32_t find_safe_cluster() {
    uint32_t cluster = 2;
    uint32_t lba = FIRST_DATA_SECTOR + (cluster - 2)*SECTORS_PER_CLUSTER;

    while (lba < FIRST_DATA_SECTOR + RESERVED_SECTORS) {
        cluster++;
        lba = FIRST_DATA_SECTOR + (cluster - 2)*SECTORS_PER_CLUSTER;
    }
    return cluster;
}

static void fat_set_entry_full(uint32_t cluster, uint16_t value) {
    if (cluster < 2) return;
    fat_read_full();
    uint32_t idx = cluster * 2; // two bytes per FAT16 entry
    fatbuf[idx]     = (uint8_t)(value & 0xFF);
    fatbuf[idx + 1] = (uint8_t)((value >> 8) & 0xFF);
    fat_write_full_to_both();
}

void create_directory(const char* name , uint16_t parent_cluster, int is_root_parent) {

    
    uint32_t new_cluster = find_safe_cluster();
    if (new_cluster == 0xFFFFFFFF) return; //if !cluster can be found

    // mark cluster as EOF 
    fat_set_entry_full(new_cluster, 0xFFFF);

    // 
    uint32_t parent_lba;
    if (parent_cluster == 0) {
        // root dir
        uint32_t out_lba;
        int out_off;
        if (find_free_root_slot(&out_lba, &out_off) != 0) return; // no space
        read_sector(out_lba, sector_buf);

        // clear entry and fill
        for (int i = 0; i < 32; i++) sector_buf[out_off + i] = 0x00;
        memorycpy(&sector_buf[out_off], (const uint8_t*)name, 8);
        // extension empty cuz its a dir
        sector_buf[out_off + 8] = ' ';
        sector_buf[out_off + 9] = ' ';
        sector_buf[out_off + 10] = ' ';
        sector_buf[out_off + 11] = 0x10; // directory attr
        sector_buf[out_off + 26] = (uint8_t)(new_cluster & 0xFF);
        sector_buf[out_off + 27] = (uint8_t)((new_cluster >> 8) & 0xFF);


    
        write_sector(out_lba, sector_buf);


    } else {
        //in future i need to add mkdirs for non root directories
    }

    // add . and .. to dir (self and parent like godot nodes lol)
    memoryset(sector_buf, 0, SECTOR_SIZE);

    // .
    memoryset(sector_buf, ' ', 11);
    memorycpy(sector_buf, (const uint8_t*)".       ", 8); // ".       "
    memorycpy(sector_buf + 8, (const uint8_t*)"   ", 3);
    sector_buf[11] = 0x10;
    sector_buf[26] = (uint8_t)(new_cluster & 0xFF);
    sector_buf[27] = (uint8_t)((new_cluster >> 8) & 0xFF);

    // ..
    memoryset(sector_buf + 32, ' ', 11);
    memorycpy(sector_buf + 32, (const uint8_t*)"..      ", 8);
    memorycpy(sector_buf + 32 + 8, (const uint8_t*)"   ", 3);
    sector_buf[32 + 11] = 0x10;
    sector_buf[32 + 26] = (uint8_t)(parent_cluster & 0xFF);
    sector_buf[32 + 27] = (uint8_t)((parent_cluster >> 8) & 0xFF);

    uint32_t new_dir_lba = FIRST_DATA_SECTOR + (new_cluster - 2) * SECTORS_PER_CLUSTER;
    write_sector(new_dir_lba, sector_buf);
}



void fat16_init() {
    //format disc
    write_fats();
    init_root_dir();
    write_test_file();

}
char* conv_fat_name(const char* input) {
    static char out[11];
    for (int k = 0; k < 11; k++) out[k] = ' ';

    int i = 0, j = 0;
    while (input[i] != '\0' && j < 8) {
        char c = input[i++];
        out[j++] = c;
    }
   
    return out;
}



void list_directory(uint16_t dir_cluster) {
    //works fire
    uint32_t lba;

    if (dir_cluster == 0)
        lba = FIRST_ROOT_DIR_SECTOR;
    else
        lba = FIRST_DATA_SECTOR + (dir_cluster - 2) * SECTORS_PER_CLUSTER;

    read_sector(lba, sector_buf);

    for (int i = 0; i < SECTOR_SIZE; i += 32) {
        if (sector_buf[i] == 0x00) break; // no more entries
        if (sector_buf[i] == 0xE5) continue; // deleted

        char name[12];
        memorycpy(name, &sector_buf[i], 11);
        name[11] = '\0';

        print(name);
        print("\n");
    }
}


void list_terminal_directory(Window* win, uint16_t dir_cluster) {

    uint32_t lba;

    if (dir_cluster == 0)
        lba = FIRST_ROOT_DIR_SECTOR;
    else
        lba = FIRST_DATA_SECTOR + (dir_cluster - 2) * SECTORS_PER_CLUSTER;

    read_sector(lba, sector_buf);

    for (int i = 0; i < SECTOR_SIZE; i += 32) {
        if (sector_buf[i] == 0x00) break; // no more entries
        if (sector_buf[i] == 0xE5) continue; // deleted

        char name[12];
        memorycpy(name, &sector_buf[i], 11);
        name[11] = '\0';

        terminal_print(win,name);
        terminal_print(win,"\n");
    }
}













uint16_t get_fat_entry(uint16_t cluster) {
    fat_read_full();
    uint32_t idx = cluster * 2;
    uint16_t val = fatbuf[idx] | (fatbuf[idx + 1] << 8);

    return val;
}


uint16_t find_directory_cluster(const char* name, uint16_t parent_cluster) {
    uint8_t buf[SECTOR_SIZE];

    //if root dir
    if (parent_cluster == 0) {
        uint32_t sector = FIRST_ROOT_DIR_SECTOR;
        for (int s = 0; s < ROOT_DIR_SECTORS; s++, sector++) {
            read_sector(sector, buf);
            for (int off = 0; off < SECTOR_SIZE; off += 32) {
                //no more entries
                if (buf[off] == 0x00) {return 0xFFFF;} 
                //if deleted entry
                if (buf[off] == 0xE5) {continue;}      
                if (memorycompare(name, &buf[off], 11) == 0) {
                    uint16_t cluster = buf[off + 26] | (buf[off + 27] << 8);
                    return cluster;
                }
            }
        }
        return 0xFFFF;
    }

    //for non root dirs
    uint16_t cluster = parent_cluster;
    while (cluster >= 0x0002 && cluster < 0xFFF8) { // data cluster range
        uint32_t first_sector = FIRST_DATA_SECTOR + (cluster - 2) * SECTORS_PER_CLUSTER;
        for (int s = 0; s < SECTORS_PER_CLUSTER; s++) {
            read_sector(first_sector + s, buf);
            for (int offset = 0; offset < SECTOR_SIZE; offset += 32) {

                if (buf[offset] == 0x00) return 0xFFFF; 
                
                if (buf[offset] == 0xE5) continue;

                if (memorycompare(name, &buf[offset], 11) == 0) {
                    uint16_t found_dir = buf[offset + 26] | (buf[offset + 27] << 8);
                    return found_dir;
                }
            }
        }
        cluster = get_fat_entry(cluster);
    }

    return 0xFFFF;  
}