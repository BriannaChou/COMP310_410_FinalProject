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
    char *file_content = nalloc(sizeof(char) * 100000);
	struct elf64_header *elf_file_hdr = file_content;
	// write function in fat.c to find file size (should be in iNode)

    	// 4. Call fatRead() to read the file's contents into your temporary bufffer.
	fatRead(fp, file_content, 100000);

    	// 5. Loop through the program headers in the ELF file and (a) map a physical page for each program header and (b) copy the program header's contents into the space you mapped.
	struct elf_program_header *prog_header = file_content + elf_file_hdr->e_phoff;
	int i;
	struct ppage *program_ppages = allocate_physical_pages(file_content->e_phnum);
	for(i = 0; i < (file_content->e_phnum * file_content->e_phentsize); i = i + file_content->e_phentsize) {
		prog_header = prog_header + i;
        // WARNING: check to see if prog_header->vaddr is already mapped. if so, don't map it. If it's not already mapped, then call mapPages and map it.
		mapPages(prog_header->vaddr, prog_header->paddr);

        // TODO: memcpy from prog_header->offset to prog_header->vaddr

		program_ppages = program_ppages->next;
	}

    	// 6. Free the temporary space you allocated in step 3.
	nfree(file_content);

    // Allocate a page for the stack and put the top of stack in the SP register
    char *top_of_stack = 0x800000;
    asm ("mov sp, %0"
         : "r"(top_of_stack)
         :
         :);
    	// 7. Jump to the entry point
        asm("bx %0"
            : "r"(elf_file_hdr->e_entry)
            :
            : );
}

