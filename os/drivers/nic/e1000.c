
#include "../include/e1000.h"
#include "../include/pci.h"
#include "../../libary/include/stdconsole.h"
#include "../../libary/include/util.h"
#include "../../network/include/ethernet.h"

static struct rx_desc rx_ring[RX_DESC_COUNT];
static uint8_t rx_buffers[RX_DESC_COUNT][RX_BUFFER_SIZE];

static uint32_t *e1000_mmio = 0;

static struct tx_desc tx_ring[TX_DESC_COUNT];
static uint8_t tx_buffers[TX_DESC_COUNT][TX_BUFFER_SIZE];
static int tx_tail = 0;



int e1000_init() {
    //find nic using pci
    struct pci_device *dev = pci_find_device(0x8086, 0x100E);
    if (!dev) {
        print("e1000 NIC not found\n");
        return -1;
    }

    //map BAR0
    e1000_mmio = (uint32_t *)(dev->bar[0] & 0xFFFFFFF0);

    //reset device
    e1000_mmio[E1000_CTRL/4] |= E1000_CTRL_RST;
    while (e1000_mmio[E1000_CTRL/4] & E1000_CTRL_RST);

    //setup rx ring
    for (int i = 0; i < RX_DESC_COUNT; i++) {
        rx_ring[i].addr = (uint64_t)&rx_buffers[i];
        rx_ring[i].status = 0;
    }


    //setup tx ring
    for (int i = 0; i < TX_DESC_COUNT; i++) {
        tx_ring[i].addr = (uint64_t)&tx_buffers[i];
        tx_ring[i].cmd = 0;
        tx_ring[i].status = E1000_TXD_STAT_DD; 
    }

    //write rx regs
    e1000_mmio[E1000_RDBAL/4] = (uint32_t)(uint64_t)rx_ring;
    e1000_mmio[E1000_RDBAH/4] = (uint32_t)((uint64_t)rx_ring >> 32);
    e1000_mmio[E1000_RDLEN/4] = sizeof(rx_ring);
    e1000_mmio[E1000_RDH/4] = 0;
    e1000_mmio[E1000_RDT/4] = RX_DESC_COUNT - 1;


    //write tx regs
    e1000_mmio[E1000_TDBAL/4] = (uint32_t)(uint64_t)tx_ring;
    e1000_mmio[E1000_TDBAH/4] = (uint32_t)((uint64_t)tx_ring >> 32);
    e1000_mmio[E1000_TDLEN/4] = sizeof(tx_ring);
    e1000_mmio[E1000_TDH/4] = 0;
    e1000_mmio[E1000_TDT/4] = 0;

    //enable RX
    e1000_mmio[E1000_RCTL/4] |= E1000_RCTL_EN;

    //enable TX
    e1000_mmio[E1000_TCTL/4] = E1000_TCTL_EN | E1000_TCTL_PSP;

    print("e1000 initialized\n");
    return 0;
}

int send_packet(void* data, uint16_t len) {
    //ret -1 if busy
    //ret 0 if sent
    int index = tx_tail;

    if (!(tx_ring[index].status & E1000_TXD_STAT_DD)) {

        return -1;
    }


    memorycpy(tx_buffers[index], data, len);

    tx_ring[index].length = len;
    tx_ring[index].cmd =
        E1000_TXD_CMD_EOP |  //packet end
        E1000_TXD_CMD_IFCS | //add checksum
        E1000_TXD_CMD_RS;    //status

    tx_ring[index].status = 0; //

    tx_tail = (tx_tail + 1) % TX_DESC_COUNT;
    e1000_mmio[E1000_TDT/4] = tx_tail;

    return 0;
}

void e1000_poll() {
    static uint32_t rx_index = 0;
    

    while (rx_ring[rx_index].status & E1000_RXD_STAT_DD) {
        uint8_t* pkt = rx_buffers[rx_index];
        uint16_t len = rx_ring[rx_index].length;

        ethernet_receive(pkt, len);

        rx_ring[rx_index].status = 0;


        rx_index = (rx_index + 1) % RX_DESC_COUNT;

        e1000_mmio[E1000_RDT/4] = rx_index;
    }
}