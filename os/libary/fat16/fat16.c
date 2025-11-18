#include <stdint.h>
#include "../include/fat16.h"
#include "../include/util.h"
#include "../../drivers/include/ata.h"
#include "../include/stdconsole.h"
#include "../../apps/terminal/terminal.h"
uint8_t sector_buf[SECTOR_SIZE];




static int find_free_root_slot(uint32_t *out_sector_lba, int *out_offset_in_sector) {
    //ret 0 found | ret -1 not found
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
    return -1;
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
    fat_read_full();
    uint32_t total_entries = (SECTORS_PER_FAT * SECTOR_SIZE) / 2;
    for (uint32_t c = 2; c < total_entries; c++) {
        uint32_t idx = c * 2;
        uint16_t entry = fatbuf[idx] | (fatbuf[idx + 1] << 8);
        if (entry == 0x0000) return c;
    }
    return 0xFFFFFFFF;
}

static uint32_t allocate_free_cluster() {
    fat_read_full();
    uint32_t total_entries = (SECTORS_PER_FAT * SECTOR_SIZE) / 2; 
    for (uint32_t c = 2; c < total_entries; c++) {
        uint32_t idx = c * 2;
        uint16_t entry = fatbuf[idx] | (fatbuf[idx + 1] << 8);
        if (entry == 0x0000) {
            //mark temporarily as EOF write actual chain later
            fatbuf[idx] = 0xFF;
            fatbuf[idx + 1] = 0xFF;
            fat_write_full_to_both();
            return c;
        }
    }
    return 0xFFFFFFFF;
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



//file creation 


// Convert 11-byte FAT format 
void make_fat11_name(const char* input, uint8_t out[11]) {
    int i;
    for (i = 0; i < 11; i++) out[i] = ' ';
    int j = 0;
    // copy name part up to '.'
    int k = 0;
    while (input[k] && input[k] != '.' && j < 8) {
        out[j++] = input[k++];
    }
    if (input[k] == '.') {
        k++;
        int extpos = 8;
        int e = 0;
        while (input[k] && e < 3) {
            out[extpos + e] = input[k++];
            e++;
        }
    }
}


static void free_cluster_chain(uint16_t start_cluster) {
    if (start_cluster < 2 || start_cluster >= 0xFFF8) return;
    fat_read_full();
    uint16_t cluster = start_cluster;
    while (cluster >= 2 && cluster < 0xFFF8) {
        uint32_t idx = cluster * 2;
        uint16_t next = fatbuf[idx] | (fatbuf[idx + 1] << 8);
        //clear this entry
        fatbuf[idx] = 0x00;
        fatbuf[idx + 1] = 0x00;
        if (next >= 0xFFF8 || next == 0x0000) break;
        cluster = next;
    }
    fat_write_full_to_both();
}




int fat_create_file_root(const char* name, const uint8_t* data, uint32_t size) {
    uint32_t dir_sect = 0;
    int dir_off = -1;
    uint16_t old_start_cluster = 0;
    uint8_t fatname[11];
    make_fat11_name(name, fatname);

    //Search root for existing file with same 11-byte name
    uint32_t sector = FIRST_ROOT_DIR_SECTOR;
    int found_existing = 0;
    for (int s = 0; s < ROOT_DIR_SECTORS; s++, sector++) {
        read_sector(sector, sector_buf);
        for (int off = 0; off < SECTOR_SIZE; off += 32) {
            uint8_t first = sector_buf[off];
            if (first == 0x00) {
                // no more entries
                s = ROOT_DIR_SECTORS; // break outer
                break;
            }
            if (first == 0xE5) continue;
            if (memorycompare((const char*)fatname, (const char*)&sector_buf[off], 11) == 0) {
                // existing file found
                dir_sect = FIRST_ROOT_DIR_SECTOR + s;
                dir_off = off;
                old_start_cluster = sector_buf[off + 26] | (sector_buf[off + 27] << 8);
                found_existing = 1;
                break;
            }
        }
        if (found_existing) break;
    }

    //If not found find a free root slot
    if (!found_existing) {
        if (find_free_root_slot(&dir_sect, &dir_off) != 0) return -1; // no free slot
    } else {
        // free previous cluster chain to avoid leaks (if it had clusters)
        if (old_start_cluster >= 2 && old_start_cluster < 0xFFF8) {
            free_cluster_chain(old_start_cluster);
        }
    }

    //allocate clusters needed
    uint32_t bytes_per_cluster = SECTORS_PER_CLUSTER * SECTOR_SIZE;
    uint32_t clusters_needed = (size + bytes_per_cluster - 1) / bytes_per_cluster;
    if (clusters_needed == 0) clusters_needed = 1;

    fat_read_full();
    uint32_t total_entries = (SECTORS_PER_FAT * SECTOR_SIZE) / 2;

    uint32_t prev_cluster = 0;
    uint32_t first_cluster = 0;

    for (uint32_t i = 0; i < clusters_needed; i++) {
        uint32_t found = 0xFFFFFFFF;
        for (uint32_t c = 2; c < total_entries; c++) {
            uint32_t idx = c * 2;
            uint16_t entry = fatbuf[idx] | (fatbuf[idx + 1] << 8);
            if (entry == 0x0000) { found = c; break; }
        }
        if (found == 0xFFFFFFFF) {
            //out of space
            return -1;
        }
        uint32_t fidx = found * 2;
        //mark temporarily as EOF
        fatbuf[fidx] = 0xFF;
        fatbuf[fidx + 1] = 0xFF;

        if (prev_cluster != 0) {
            uint32_t pidx = prev_cluster * 2;
            fatbuf[pidx] = (uint8_t)(found & 0xFF);
            fatbuf[pidx + 1] = (uint8_t)((found >> 8) & 0xFF);
        } else {
            first_cluster = found;
        }
        prev_cluster = found;
    }

    //ensure EOF for last cluster already 0xFFFF
    fat_write_full_to_both();

    //write data into allocated clusters
    uint32_t remaining = size;
    const uint8_t* src = data;
    uint32_t current_cluster = first_cluster;
    while (remaining > 0) {
        uint32_t lba = FIRST_DATA_SECTOR + (current_cluster - 2) * SECTORS_PER_CLUSTER;
        for (int s = 0; s < SECTORS_PER_CLUSTER; s++) {
            uint8_t dbuf[SECTOR_SIZE];
            uint32_t tocopy = remaining > SECTOR_SIZE ? SECTOR_SIZE : remaining;
            memoryset(dbuf, 0, SECTOR_SIZE);
            if (tocopy > 0) {
                memorycpy(dbuf, src, tocopy);
                src += tocopy;
                remaining -= tocopy;
            }
            write_sector(lba + s, dbuf);
        }
        uint16_t next = get_fat_entry(current_cluster);
        if (next >= 0xFFF8 || remaining == 0) break;
        current_cluster = next;
    }

    //fill directory entry (dir_sect/dir_off is set either to existing entry or free slot)
    read_sector(dir_sect, sector_buf);
    //clear entry first 32 bytes
    for (int i = 0; i < 32; i++) sector_buf[dir_off + i] = 0x00;

    for (int i = 0; i < 11; i++) sector_buf[dir_off + i] = fatname[i];
    sector_buf[dir_off + 11] = 0x20; //archive attribute

    sector_buf[dir_off + 26] = (uint8_t)(first_cluster & 0xFF);
    sector_buf[dir_off + 27] = (uint8_t)((first_cluster >> 8) & 0xFF);

    sector_buf[dir_off + 28] = (uint8_t)(size & 0xFF);
    sector_buf[dir_off + 29] = (uint8_t)((size >> 8) & 0xFF);
    sector_buf[dir_off + 30] = (uint8_t)((size >> 16) & 0xFF);
    sector_buf[dir_off + 31] = (uint8_t)((size >> 24) & 0xFF);

    write_sector(dir_sect, sector_buf);

    return 0;
}