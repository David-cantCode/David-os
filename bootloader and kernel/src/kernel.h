#ifndef KERNEL_H
#define KERNEL_H

void kernel_main();
void clear_screen();
void print_string(const char* string, unsigned char attr, int x, int y);
void print_string(const char* string, unsigned char attribute, int posX, int posY);
#endif