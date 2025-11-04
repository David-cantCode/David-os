#include "../include/util.h"


void memory_copy(unsigned char *source, unsigned char *dest, unsigned long no_of_bytes) {
    int i;
    for (i = 0; i < no_of_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

int string_length(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        ++i;
    }
    return i;
}

void append(char s[], char n) {
    int len = string_length(s);
    s[len] = n;
    s[len + 1] = '\0';
}

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void *memoryset(void *ptr, int value, unsigned int num) {
    unsigned char *p = (unsigned char *)ptr;
    for (unsigned int i = 0; i < num; i++) {
        p[i] = (unsigned char)value;
    }
    return ptr;
}
void *memorycpy(void *dst, const void *src, unsigned int num) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    for (unsigned int i = 0; i < num; i++) {
        d[i] = s[i];
    }
    return dst;
}

int memorycompare(const void *ptr1, const void *ptr2, uint32_t n) {
    const uint8_t *a = (const uint8_t*)ptr1;
    const uint8_t *b = (const uint8_t*)ptr2;

    for (uint32_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return (a[i] < b[i]) ? -1 : 1;
        }
    }
    return 0;
}

void reverse_str(char* str, int len) {
    int i = 0, j = len - 1;
    while (i < j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++; j--;
    }
}

void int_to_str(int num, char* buffer) {
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % 10;
        buffer[i++] = '0' + rem;
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    reverse_str(buffer, i);
}

int absolute(int x) {
    return x < 0 ? -x : x;
}

void *memorymove(void *dest, const void *src, unsigned int n) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    if (d < s) {
        for (unsigned int i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else if (d > s) {
 
        for (unsigned int i = n; i-- > 0;) {
            d[i] = s[i];
        }
    }
  
    return dest;
}
