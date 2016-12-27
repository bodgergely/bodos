#ifndef STRING_H
#define STRING_H
#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* str);
char* strcpy(char *dest, const char *src);
int isspace(const char c);

char* itoa (unsigned int val, char *s, int base);

void* memset(void* s, int c, size_t n);

#endif
