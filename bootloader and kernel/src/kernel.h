#ifndef KERNEL_H
#define KERNEL_H

void kernel_main();
void clear_screen();
void print_string(char* string, unsigned char attribute, int posX, int posY);
void print_char(char character, unsigned char attribute, int posX, int posY);
#endif