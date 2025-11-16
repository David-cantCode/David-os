#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>


void ethernet_init(uint8_t* mac);

void ethernet_receive(void* packet, uint16_t len);

extern uint8_t local_mac[6];

#endif
