#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "page.h"

typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

struct process {
	struct process *next;
	struct process *prev;
	uint32_t pid;
	char path[16];
};

int exec(char *path);

#endif
