#include "nalloc.h"
#include "fat.h"
#include "elf.h"
#include "page.h"
#include "mmu.h"

int exec(char *path) {

    	// 1. nalloc() a new process structure and add it to a linked list of active processes.
	struct block *activeProcessListHead;
	struct process currentProcess = nalloc(sizeof(struct process));
	listInsert(&activeProcessListHead, currentProcess);

    	// 2. Call fatOpen() to open the file given by path
	struct file fp;
	fatOpen(&fp, path);

    	// 3. Allocate some memory to load the file's contents. you can use nalloc() or you can temporarily map a memory page with mapPages().
	struct elf32_header *file_content = nalloc(sizeof(char) * 100000);
	// write function in fat.c to find file size (should be in iNode)

    	// 4. Call fatRead() to read the file's contents into your temporary bufffer.
	fatRead(fp, file_content, 100000);

    	// 5. Loop through the program headers in the ELF file and (a) map a physical page for each program header and (b) copy the program header's contents into the space you mapped.
	struct elf_program_header *prog_header = fp +  file_content->e_phoff;
	int i;
	struct ppage *program_ppages = allocate_physical_pages(file_content->e_phnum);
	for(i = 0; i < (file_content->e_phnum * file_content->e_phentsize); i = i + file_content->e_phentsize) {
		prog_header = prog_header + i;
		mapPages(prog_header->vaddr, prog_header->paddr);
		program_ppages = program_ppages->next;
		program_ppages 	
	}

    	// 6. Free the temporary space you allocated in step 3.
	nfree(file_content);
    	// 7. Jump to the entry point
}

