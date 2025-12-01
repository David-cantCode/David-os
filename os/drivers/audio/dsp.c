//DSP - Digital signal proccesor

#include "../../libary/include/stdport.h"


//ports
#define mixer_port 0x224
#define mixer_data 0x225
#define reset 0x226
#define read 0x22A
#define write 0x22C
#define status 0x22E


//write commands
#define set_time 0x40
#define set_rate 0x41

#define speaker_on 0xD1
#define speaker_off 0xD3

#define stop_8bit 0xD0
#define resume_8bit 0xD4

#define stop_16bit 0xD5
#define resume_16bit 0xD6


//mixer port commands
#define master_volume 0x22


void innit_dsp(){


    //reset dsp
    outb(reset, 1);
    outb(reset, 0);


    
}

