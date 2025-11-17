// kernel.c AUG 14


#include "cpu/idt.c"
#include "../libary/include/stdconsole.h"
#include "cpu/isr.h"
#include "shell/shell.c"
#include "../drivers/include/timer.h"
#include "../drivers/include/e1000.h"
#include "../libary/include/program.h"
#include "../drivers/include/pci.h"
#include "../drivers/include/mouse.h"



#include "../network/include/arp.h"
#include "../network/include/icmp.h"
#include "../network/include/ethernet.h"
volatile int key_down;


extern uint32_t fb_addr;
extern uint32_t pitch;
extern uint32_t screen_width;
extern uint32_t screen_height;
#define VBE_ADDR 0x400
#define INTERUPT_FREQ 100


Program *Shell;



#define MAX_PROGRAMS 32
volatile Program* programs[MAX_PROGRAMS];
volatile int program_count;


void kernel_main(){

    //*******************
    // *****MAIN*******
    // ***************** 
    asm volatile("sti");
    while (1) {
        // pause till interupt
        asm volatile("hlt");
        if (key_down == 1) {
            shell_main(scancode);
            key_down = 0;        
    }

    

    
    }
}


void get_e1000(){
    e1000_init();   
    
    uint8_t mac[6] = {0x52,0x54,0x00,0x12,0x34,0x56};
    ethernet_init(mac);
    uint32_t host_ip = 0xC0A86401; 


    uint16_t seq = 1;

        e1000_poll();
        for (volatile int i=0;i<100000000;i++);

}


void kernel_ini() {
    //**************************
    // *********INITIALLIZE*****
    // *************************
     __asm__ volatile ("cli");

    uint32_t* vbe = (uint32_t*)VBE_ADDR;

    fb_addr       = vbe[0]; // 0x0400
    screen_width  = vbe[1]; // 0x0404
    screen_height = vbe[2]; // 0x0408
    pitch         =  2560; //pitch ≈ width * (bits_per_pixel / 8)




    //char msgKernel[] = "Kernel was loaded";

    print("kernel was loaded \n");

    
    IDT_Initialize();
    print("Interupt Descriptor Table was loaded\n");


    for (volatile int i=0;i<1000000000;i++);

    ISR_Initialize();
    print("PIC was Remaped \n");


    timer_set_frequency(INTERUPT_FREQ);

    print("Detecting Devices\n");
    for (volatile int i=0;i<10000000;i++);
    pci_enumerate();
    print("\n");

    

    get_e1000();


    print("setting up mouse\n");
    mouse_init();


    print("Shell was Loaded \n");
    print("\n");

    for (volatile int i=0;i<100000000;i++);




    print("\n");
    print("=============USER SPACE============\n\n");



    Shell = create_program(PROGRAM_SHELL, shell_ini, 0,0,0,0);


    kernel_main();
}

