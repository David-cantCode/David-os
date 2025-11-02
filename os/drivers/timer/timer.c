
#include "../../libary/include/stdport.h"
#include "../include/timer.h"

volatile unsigned int tick = 0;




#define PIT_FREQ 1193182

void timer_set_frequency(unsigned int freq) {
    //ex: timer_set..(100) 100hz - gives 1 tick every 10 ms

    unsigned int divisor = PIT_FREQ / freq;
    outb(0x43, 0x36);             
    outb(0x40, divisor & 0xFF);   
    outb(0x40, (divisor >> 8) & 0xFF); 
}


void timer_callback() {
    tick++;
  
}
