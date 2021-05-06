#include "mmu.h"


struct table_descriptor_stage1 L1table[512] __attribute__((aligned(4096)));
struct page_descriptor_stage1 L2table[512] __attribute__((aligned(4096)));

/*

   38             30  29                 21  20                           0
  |------------------|----------------------|------------------------------|
  | L1 table entry   |  L2 table entry      | Offset                       |
  |------------------|----------------------|------------------------------|

*/


void mapPages(void *vaddr, void *paddr) {
    unsigned int L2tableIndex = ((unsigned int)vaddr >> 21) & 0x1ff;
    unsigned int L1tableIndex = ((unsigned int)vaddr >> 30) & 0x1ff;


    L1table[L1tableIndex].type = 3;
    L1table[L1tableIndex].next_lvl_table = ((unsigned int)&L2table[0])>>12;

    L2table[L2tableIndex].attrindx = 0; // Normal memory, not memory-mapped IO 1 for IO
    L2table[L2tableIndex].type = 1; // Pointing to memory page
    L2table[L2tableIndex].sh = 3; // Set inner sharable
    L2table[L2tableIndex].ap = 0; // Access permission, kernel RW
    L2table[L2tableIndex].af = 1; // ??
    L2table[L2tableIndex].output_addr = (unsigned int)paddr | 0x80000; //NEED TO FIX
    
}






/*
 * loadPageTable
 *
 * Enables the MMU, loading the given L1 page table.
 *
 */
int loadPageTable(struct table_descriptor_stage1 *L1table) {

    unsigned long b, r;
    // Enable paging in system regs. This code should be moved elsewhere. Testing only for now.
    // check for 4k granule and at least 36 bits physical address bus */
    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b=r&0xF;
    if(r&(0xF<<28)/*4k*/ || b<1/*36 bits*/) {
//        uart_puts("ERROR: 4k granule or 36 bit address space not supported\n");
        return -1;
    }   
    // first, set Memory Attributes array, indexed by PT_MEM, PT_DEV, PT_NC in our example
    r=  (0xFF << 0) |    // AttrIdx=0: normal, IWBWA, OWBWA, NTR
        (0x04 << 8) |    // AttrIdx=1: device, nGnRE (must be OSH too)
        (0x44 <<16);     // AttrIdx=2: non cacheable
    asm volatile ("msr mair_el1, %0" : : "r" (r));

    // next, specify mapping characteristics in translate control register
    r=  (0b00LL << 37) | // TBI=0, no tagging
        (b << 32) |      // IPS=autodetected
        (0b10LL << 30) | // TG1=4k
        (0b11LL << 28) | // SH1=3 inner
        (0b01LL << 26) | // ORGN1=1 write back
        (0b01LL << 24) | // IRGN1=1 write back
        (0b0LL  << 23) | // EPD1 enable higher half
        (25LL   << 16) | // T1SZ=25, 3 levels (512G)
        (0b00LL << 14) | // TG0=4k
        (0b11LL << 12) | // SH0=3 inner
        (0b01LL << 10) | // ORGN0=1 write back
        (0b01LL << 8) |  // IRGN0=1 write back
        (0b0LL  << 7) |  // EPD0 enable lower half
        (25LL   << 0);   // T0SZ=25, 3 levels (512G)
    asm volatile ("msr tcr_el1, %0; isb" : : "r" (r));


    // tell the MMU where our translation tables are. TTBR_CNP bit not documented, but required
    // lower half, user space
    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)L1table));

    // finally, toggle some bits in system control register to enable page translation
    asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (r));
    r|=0xC00800;     // set mandatory reserved bits
    r&=~((1<<25) |   // clear EE, little endian translation tables
         (1<<24) |   // clear E0E
         (1<<19) |   // clear WXN
         (1<<12) |   // clear I, no instruction cache
         (1<<4) |    // clear SA0
         (1<<3) |    // clear SA
         (1<<2) |    // clear C, no cache at all
         (1<<1));    // clear A, no aligment check
    r|=  (1<<0);     // set M, enable MMU
    asm volatile ("msr sctlr_el1, %0; isb" : : "r" (r));
    return 0;
}

int isMapped(void *vaddr) {
	unsigned int L2tableIndex = ((unsigned int)vaddr >> 21) & 0x1ff;
    	unsigned int L1tableIndex = ((unsigned int)vaddr >> 30) & 0x1ff;
	if(L2table[L2tableIndex].type == 0) {
		return 0;
	}
	return 1;
}


