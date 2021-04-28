
//#include <string.h>
#include "clibfuncs.h"

/*
 * C Library Routines 
 *
 *
 */

size_t strlen(const char *str) {
    unsigned int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

int tolower(int c) {
    if(c < 'a') { // Check if c is uppercase
        c -= 'a' - 'A';
    }
    return c;
}

int isdig(int c) {
    if((c >= '0') && (c <= '9')){
        return 1;
    } else {
        return 0;
    }
}

void * memcpy(void * __restrict dest, const void * __restrict src, size_t num) {

    unsigned char *d = (unsigned char*)dest;
    unsigned char *s = (unsigned char*)src;
    unsigned int k;

    for(k = 0; k < num; k++){
        d[k] = s[k];
    }

    return dest;
}


void *memset(void *s, int c, size_t n){

    unsigned char *ptr = (unsigned char*)s;
    unsigned char v = (unsigned char)c;
    int i;

    for(i = 0; i < n; i++) {
        ptr[i] = v;
    }

    return s;

}

int strcmp ( const char * str1, const char * str2 ) {

    int k = 0;

    do {
        k++;
    } while((str1[k] == str2[k]) && (str1[k] != '\0') && (str2[k] != '\0'));

    if(str1[k] < str2[k]){
        return -1;
    } else if(str1[k] > str2[k]){
        return 1;
    } else {
        return 0;
    }
}


int strncmp ( const char * str1, const char * str2, size_t n ) {

    int k = 0;

    do {
        k++;
    } while((str1[k] == str2[k]) && (str1[k] != '\0') && (str2[k] != '\0') && (k < n));

    if(k == n){
        return 0;
    }

    if(str1[k] < str2[k]){
        return -1;
    } else if(str1[k] > str2[k]){
        return 1;
    } else {
        return 0;
    }
}

int toupper(int c){
    if(c >= 'a' && c <= 'z'){
        c -= 'a' - 'A';
    }
    return c;
}

char * strcpy ( char * destination, const char * source ) {

    int k = 0;
    while(source[k] != '\0'){
        destination[k] = source[k];
        k++;
    }
    destination[k] = '\0';
    return destination;
}

//to do
int execv(const char *path, char *const argv[]){
    //load file (using path) with FAT
    //parse with elf (elf.h)
        //find or create segments (data, stack, bss, etc)
    //allocate pages (pages.h)
    //map pages to virtual memory
    //find entry point and jump to it to start running
        //start
}


