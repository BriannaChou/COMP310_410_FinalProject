#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "page.h"

struct process {
	struct process *next;
	struct process *prev;
	uint32_t pid;
	char path[16];
};

int exec(char *path);

#endif
