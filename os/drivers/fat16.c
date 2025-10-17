






#include <stdint.h>

#define FILENAME_LEN 11

typedef struct {
    char filename[FILENAME_LEN];  // "HELLO   TXT"
    uint16_t first_cluster;       // Where file data starts
    uint32_t size;                // File size in bytes
} __attribute__((packed)) DirEntry;
