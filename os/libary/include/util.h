#ifndef  UTIL_H
#define UTIL_H

void memory_copy(unsigned char *source, unsigned char *dest, unsigned long no_of_bytes);
int string_length(char s[]);
void append(char s[], char n);
int compare_string(char s1[], char s2[]);
#endif