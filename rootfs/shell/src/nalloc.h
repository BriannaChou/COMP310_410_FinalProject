

/*
 * nalloc.h
 *
 *
 *
 *
 *
 *
 */


#ifndef __NALLOC_H
#define __NALLOC_H

// Number of bytes to round all allocations to
#define ALIGNSIZE 2

struct block
{
	struct block *next ; // Points to the next element in the linked list
	struct block *prev ; // Points to the previous element in the linked list
	short size ;           // Size of the block + memory
} ;


void memInit() ;
void *nalloc(unsigned int size) ;
void nfree(void *block) ;

void listInsert(struct block **listHead, struct block *b);
void listDelete(struct block *b);


#ifdef DEBUG
int getTotalBlockSize(struct block *listHead, int *numItems);
double getExpectedBlockSize(struct block *listHead);
void printHeap() ;
#endif

#endif

