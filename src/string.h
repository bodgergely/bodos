#ifndef STRING_H
#define STRING_H
#include "header.h"

size_t strlen(const char* str);
char* strcpy(char *dest, const char *src);
int isspace(const char c);

char* itoa (int val, char *s, int base);

#endif
