



int exec(char *path) {

    // 1. nalloc() a new process structure and add it to a linked list of active processes.
    // 2. Call fatOpen() to open the file given by path
    // 3. Allocate some memory to load the file's contents. you can use nalloc() or you can temporarily map a memory page with mapPages().
    // 4. Call fatRead() to read the file's contents into your temporary bufffer.
    // 5. Loop through the program headers in the ELF file and (a) map a physical page for each program header and (b) copy the program header's contents into the space you mapped.
    // 6. Free the temporary space you allocated in step 3.
    // 7. Jump to the entry point
}



