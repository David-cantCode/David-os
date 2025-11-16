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

void append(char s[], char n, int max_len) {
    int len = string_length(s);
    if (len < max_len - 1) { 
        s[len] = n;
        s[len + 1] = '\0';
    }
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


int starts_with(const char* a, const char* b) {
    while (*b) {
        if (*a++ != *b++) return 0;
    }
    return 1;
}


static void reverse(char *str, int len) {
    int i = 0, j = len - 1;
    while (i < j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++; j--;
    }
}

static int itoa(int value, char *str, int base) {
    int i = 0;
    int neg = 0;
    if (value == 0) { str[i++] = '0'; str[i] = 0; return i; }
    if (value < 0 && base == 10) { neg = 1; value = -value; }
    while (value) {
        int rem = value % base;
        str[i++] = (rem > 9) ? rem - 10 + 'a' : rem + '0';
        value /= base;
    }
    if (neg) str[i++] = '-';
    str[i] = 0;
    reverse(str, i);
    return i;
}

// sprintf-like, takes array of arguments
int s_printf(char *buf, const char *fmt, uint32_t val) {
    char *p = buf;
    char tmp[32];

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    itoa((int)val, tmp, 10);  // decimal
                    char *t = tmp;
                    while (*t) *p++ = *t++;
                    break;
                }
                case 'x': {
                    itoa((int)val, tmp, 16);  // hex
                    char *t = tmp;
                    while (*t) *p++ = *t++;
                    break;
                }
                case 'p': {  // pointer
                    itoa((unsigned long)val, tmp, 16);
                    char *t = tmp;
                    while (*t) *p++ = *t++;
                    break;
                }
                case 's': {  // string
                    char *s = (char*)(uintptr_t)val;
                    while (*s) *p++ = *s++;
                    break;
                }
                default:
                    *p++ = *fmt;  // unknown specifier: just print it
            }
        } else {
            *p++ = *fmt;
        }
        fmt++;
    }

    *p = 0;
    return p - buf;
}
