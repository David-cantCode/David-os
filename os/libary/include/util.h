#ifndef  UTIL_H
#define UTIL_H

#include <stdint.h>
void memory_copy(unsigned char *source, unsigned char *dest, unsigned long no_of_bytes);
int string_length(char s[]);
void append(char s[], char n);
int compare_string(char s1[], char s2[]);
void *memoryset(void *ptr, int value, unsigned int num);
void *memorycpy(void *dst, const void *src, unsigned int num) ;
int memorycompare(const void *ptr1, const void *ptr2, uint32_t n);




#endif