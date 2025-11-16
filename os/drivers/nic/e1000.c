
#include "../include/e1000.h"
#include "../include/pci.h"
#include "../../libary/include/stdconsole.h"
#include "../../libary/include/util.h"


static struct rx_desc rx_ring[RX_DESC_COUNT];
static uint8_t rx_buffers[RX_DESC_COUNT][RX_BUFFER_SIZE];

static uint32_t *e1000_mmio = 0;

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

    e1000_mmio[E1000_RDBAL/4] = (uint32_t)(uint64_t)rx_ring;
    e1000_mmio[E1000_RDBAH/4] = (uint32_t)((uint64_t)rx_ring >> 32);
    e1000_mmio[E1000_RDLEN/4] = sizeof(rx_ring);
    e1000_mmio[E1000_RDH/4] = 0;
    e1000_mmio[E1000_RDT/4] = RX_DESC_COUNT - 1;

    //enable RX
    e1000_mmio[E1000_RCTL/4] |= E1000_RCTL_EN;

    print("e1000 initialized\n");
    return 0;
}


