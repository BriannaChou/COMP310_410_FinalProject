#include <stddef.h>
#ifndef clibfunctions_h
#define clibfunctions_h

size_t strlen(const char *str);

int tolower(int c);
int isdig(int c);
void * memcpy(void * __restrict dest, const void * __restrict src, size_t num);
void *memset(void *s, int c, size_t n);
int strcmp ( const char * str1, const char * str2 );
int strncmp ( const char * str1, const char * str2, size_t n );
int toupper(int c);
char * strcpy ( char * destination, const char * source );

#endif
