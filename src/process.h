#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "page.h"

struct process {
	struct process *next;
	struct process *prev;
	uint32_t pid;
	struct segment *segmentList; //linked list of segments belonging to this process. Each segment has a linked list of physical pages along with a virtual address. 
		// Useful for context switching.
	struct ppage *L1table;
	struct ppage *L2table;
	uint32_t syscall_params[8]; // used in the event that a syscall has to wait for the IO;
	char path[16];
};

int exec(char *path);

#endif
