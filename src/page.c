#include "list.h"
#include "page.h"
#include "nalloc.h"

#ifndef __NULL
#define NULL (void *)0
#endif
#define NPAGES 128


extern int __end;

struct ppage physical_page_array[NPAGES];
struct ppage *free_list = NULL;

void init_pfa_list(void) {
	void *physical_addr = (void *) (((unsigned long) &__end) & ~(0x200000 - 1)) + 0x200000;
	for(int i = 0; i < NPAGES; i++) {
		physical_page_array[i].physical_addr = physical_addr;
		listAdd((struct listElement **) &free_list, (struct listElement *) &physical_page_array[i]);
		physical_addr += 0x200000;
	}
}

struct ppage *allocate_physical_page() {
	struct ppage *page;
	page = (struct ppage *) nalloc(sizeof(struct ppage));
	return page;
}

struct ppage *allocate_physical_pages(unsigned int npages) {
	struct ppage *next_element = free_list;
	struct ppage *allocated_list = NULL;
	for(int i = 0; i < npages; i++) {
		listRemove((struct listElement *) next_element);
		listAdd((struct listElement **) &allocated_list, (struct listElement *) next_element);
	}
	return allocated_list;
}

void free_physical_pages(struct ppage *ppage_list){
	struct ppage *next_element = ppage_list;
	while(ppage_list != NULL) {
		listRemove((struct listElement *) next_element);
		listAdd((struct listElement **) &free_list, (struct listElement *)next_element);
	}
}
