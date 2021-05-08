#include "process.h"
#include "nalloc.h"
#include "clibfuncs.h"
#include "nfat.h"
#include "elf.h"
#include "page.h"
#include "mmu.h"
#include "rprintf.h"
#include "serial.h"

int exec(char *path) {

    	// 1. nalloc() a new process structure and add it to a linked list of active processes.
	struct block *active_process_list_head;
	struct process *current_process;
	current_process =(struct process *) nalloc(sizeof(struct process));
	listInsert(&active_process_list_head, (struct block *) current_process);

    	// 2. Call fatOpen() to open the file given by path
	struct file fp;
	fatOpen(path, &fp);
	esp_printf((void *) putc, "File opened!\n");

    	// 3. Allocate some memory to load the file's contents. you can use nalloc() or you can temporarily map a memory page with mapPages().
    	char *file_content;
        file_content = (char *) nalloc(fatGetFileSize(&fp));
	struct elf64_header *elf_file_hdr = (struct elf64_header *) file_content;
	// write function in fat.c to find file size (should be in iNode)

    	// 4. Call fatRead() to read the file's contents into your temporary bufffer.
	fatRead(&fp, file_content, fatGetFileSize(&fp));
	esp_printf((void *) putc, "File read complete!\n");
	

    	// 5. Loop through the program headers in the ELF file and (a) map a physical page for each program header and (b) copy the program header's contents into the space you mapped.
	struct elf64_program_header *elf_prog_header = (struct elf64_program_header *) (file_content + elf_file_hdr->e_phoff);

	int i;
	struct ppage *program_ppage;
	for(i = 0; i < elf_file_hdr->e_phnum; i++) {
        	// WARNING: check to see if prog_header->vaddr is already mapped. if so, don't map it. If it's not already mapped, then call mapPages and map it.
		if(!isMapped((void *) elf_prog_header[i].vaddr)) {
			program_ppage = allocate_physical_page();
			mapPages((void *) elf_prog_header[i].vaddr, (void *) elf_prog_header[i].paddr);

			// (b) copy the program header's contents into the mapped space
			memcpy((void *) elf_prog_header[i].vaddr, (void *) elf_prog_header[i].offset, elf_prog_header[i].filesz);
			// link neighbor physical pages
			struct ppage *prev_ppage = program_ppage;
			program_ppage = program_ppage->next;
			program_ppage->prev = prev_ppage;
		}
	}

    	// 6. Free the temporary space you allocated in step 3.
	nfree(file_content);

	// Allocate a page for the stack and put the top of stack in the SP register
    	char * top_of_stack = (char *) 0x800000;
    	asm ("mov sp, %0"
		       	: "=r" (top_of_stack)
			:);
    	// 7. Jump to the entry point
        asm("br %0"
			: "=r" (elf_file_hdr->e_entry)
			:);

	return 0;
}
