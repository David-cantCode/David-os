#ifndef E1000_H
#define E1000_H

#include <stdint.h>

// e1000 registers offsets
#define E1000_CTRL   0x0000
#define E1000_CTRL_RST 0x04000000



#define RX_DESC_COUNT 8
#define RX_BUFFER_SIZE 2048

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


//tx registers

#define TX_DESC_COUNT 8
#define TX_BUFFER_SIZE 2048

#define E1000_TDBAL  0x3800
#define E1000_TDBAH  0x3804
#define E1000_TDLEN  0x3808
#define E1000_TDH    0x3810
#define E1000_TDT    0x3818
#define E1000_TCTL   0x0400
#define E1000_TCTL_EN (1 << 1)
#define E1000_TCTL_PSP (1 << 3)

#define E1000_TXD_CMD_EOP  (1 << 0)
#define E1000_TXD_CMD_IFCS (1 << 1)
#define E1000_TXD_CMD_RS   (1 << 3)
#define E1000_TXD_STAT_DD  (1 << 0)

struct tx_desc {
    uint64_t addr;
    uint16_t length;
    uint8_t  cso;
    uint8_t  cmd;
    uint8_t  status;
    uint8_t  css;
    uint16_t special;
} __attribute__((packed));





int e1000_init();
int send_packet(void* data, uint16_t len);
#endif
