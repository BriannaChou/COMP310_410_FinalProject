#include <stddef.h>
#ifndef __CLIBS_H__
#define __CLIBS_H__


size_t strlen(const char *str);

int tolower(int c);
int isdig(int c);

void * memcpy(void * __restrict dest, const void * __restrict src, size_t num);
void *memset(void *s, int c, size_t n);
int strcmp ( const char * str1, const char * str2 );
int strncmp ( const char * str1, const char * str2, size_t n );
int toupper(int c);
char * strcpy ( char * destination, const char * source );
int execv(const char *path, char *const argv[]);

#endif
