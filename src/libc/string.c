#include <string.h>
#include <kernel/klog.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

char* strcpy(char *dest, const char *src)
{
	char* r = dest;
	while((*dest++ = *src++)!='\0');
	return r;
}

int isspace(const char c)
{
	switch(c)
	{
	case ' ':
	case '\t':
	case '\n':
	case '\a':
		return 1;
		break;
	default:
		return 0;
	}
}


char* itoa (unsigned int val, char *s, int base)
{
	(void)base;
	unsigned int n = 0;
	unsigned int c = val;
	while(c)
	{
		c = c/10;
		n++;
	}
	s[n] = '\0';

	size_t i = n-1;

	while(val)
	{
		unsigned int d = val % 10;
		s[i] = '0' + d;
		val/=10;
		--i;
	}


	return s;
}



void* memset(void* s, int c, size_t n)
{
	char* mem = (char*)s;
	char val = (char)c;
	for(int i=0;i<n;i++)
	{
		mem[i] = val;
	}

	return s;

}
