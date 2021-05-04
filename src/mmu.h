


// L1 table structure

struct table_descriptor_stage1 {
    unsigned int type             :2; // Least significant bits
    unsigned int ignored          :10;
    unsigned long next_lvl_table  :36;
    unsigned int res0             :11;
    unsigned int pxn_table        :1;
    unsigned int xn_table         :1;
    unsigned int ap_table         :2;
    unsigned int ns_table         :1; // Most significant bit
};


// L2 table structure
struct page_descriptor_stage1 {
    // lower attributes (bits 16:2)
    unsigned int type        : 2;  // block (2'b01) or table (2'b11)
    unsigned int attrindx    : 3;  // stage 1 memory attr index field
    unsigned int ns          : 1;  // non-secure
    unsigned int ap          : 2;  // data access permissions
    unsigned int sh          : 2;  // sharability field
    unsigned int af          : 1;  // accessed flag
    unsigned int ng          : 1;  // not global
    unsigned int oa          : 4;  // ?
    unsigned int nt          : 1;  // block translation entry
    unsigned int output_addr : 18; // output address. size config'd by T0SZ field in tcr_el1
    unsigned int res01       : 13; // another res0 field
    unsigned int res00       : 2;  // reserved, 0

    // upper attributes (bits 63:50)
    unsigned int gp          : 1;  // guarded page
    unsigned int dbm         : 1;  // dirty bit modifier
    unsigned int contiguous  : 1;  // translation table entry is one of a
                                   // contiguous set of entries that might be
                                   // cached in a single TLB entry
    unsigned int pxn         : 1;  // privileged execute never
    unsigned int xn          : 1;  // execute never
    unsigned int ignored2    : 4;
    unsigned int pbha        : 4;  // page-based hardware attributes

    unsigned int ignored1    : 1;
}__attribute__((packed));


void mapPages(void *vaddr, void *paddr);
int loadPageTable(struct table_descriptor_stage1 *L1table);

struct table_descriptor_stage1 L1table[512] __attribute__((aligned(4096)));
struct page_descriptor_stage1 L2table[512] __attribute__((aligned(4096)));


