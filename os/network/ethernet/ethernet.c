#include "../include/ethernet.h"
#include "../../drivers/include/e1000.h"
#include "../../libary/include/stdconsole.h"
#include "../../libary/include/util.h"
#include "../include/arp.h"
#include "../include/ipv4.h"

uint8_t local_mac[6];

void ethernet_init(uint8_t* mac) {
    memorycpy(local_mac, mac, 6);
}

void ethernet_receive(void* packet, uint16_t len) {
    print("recived packet");
}
