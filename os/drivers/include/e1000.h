#ifndef E1000_H
#define E1000_H

#include <stdint.h>
#define RX_DESC_COUNT 8
#define RX_BUFFER_SIZE 2048

// e1000 registers offsets
#define E1000_CTRL   0x0000
#define E1000_CTRL_RST 0x04000000

#define E1000_RDBAL 0x2800
#define E1000_RDBAH 0x2804
#define E1000_RDLEN 0x2808
#define E1000_RDH   0x2810
#define E1000_RDT   0x2818
#define E1000_RCTL  0x0100
#define E1000_RCTL_EN 0x2


struct rx_desc {
    uint64_t addr;
    uint16_t length;
    uint16_t checksum;
    uint8_t status;
    uint8_t errors;
    uint16_t special;
} __attribute__((packed));


int e1000_init();
#endif
