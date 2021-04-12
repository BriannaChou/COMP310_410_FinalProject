#ifndef __PAGES_H
#define __PAGES_H

struct ppage {
	struct ppage *next;
	struct ppage *prev;
	void *physical_addr;
};

void init_pfa_list(void);
struct ppage *allocate_physical_pages(unsigned int npages);
void free_physical_pages(struct ppage *ppage_list);

#endif
