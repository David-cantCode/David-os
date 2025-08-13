#include "kernel.h"

// kernel.c
void kernel_main() {
    const char *message = "Hello, World from kernel!\n";
    char *video_memory = (char*) 0xB8000;  // VGA text mode memory
    for (int i = 0; message[i] != '\0'; i++) {
        video_memory[i*2] = message[i];     // character
        video_memory[i*2+1] = 0x07;         // color (light grey on black)
    }
    while (1) { }
}
