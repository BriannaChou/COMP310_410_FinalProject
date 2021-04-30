#ifndef __ELF_H
#define __ELF_H

#define PROG_HEADER_TYPE_NULL    0
#define PROG_HEADER_TYPE_LOAD    1
#define PROG_HEADER_TYPE_DYNAMIC 2
#define PROG_HEADER_TYPE_INTERP  3
#define PROG_HEADER_TYPE_NOTE    4
#define PROG_HEADER_TYPE_SHLIB   5
#define PROG_HEADER_TYPE_PHDR    6
#define PROG_HEADER_TYPE_LOOS    0x60000000
#define PROG_HEADER_TYPE_HIOS    0x6fffffff
#define PROG_HEADER_TYPE_LOPROC  0x70000000
#define PROG_HEADER_TYPE_HIPROC  0x7fffffff

struct elf32_header {
    uint32_t  magic;          // Magic number should be 0x7f 'E' 'L' 'F'
    uint8_t   elfclass;       // 1- 32bit, 2- 64bit ELF file
    uint8_t   endianness;     // 1- Little endian, 2- Big endian
    uint8_t   elfversion;     // Version of the ELF file
    uint8_t   osabi;          // ABI
    uint8_t   osabiversion;   // ABI version
    uint8_t   pad[7];         // Unused
    uint16_t  e_type;         // Object file type
    uint16_t  e_machine;      // Architecture
    uint32_t  e_version;      // Object file version
    uint32_t  e_entry;        // Entry point virtual address
    uint32_t  e_phoff;        // Program header table file offset
    uint32_t  e_shoff;        // Section header table file offset
    uint32_t  e_flags;        // Processor-specific flags
    uint16_t  e_ehsize;       // ELF header size in bytes
    uint16_t  e_phentsize;    // Program header table entry size
    uint16_t  e_phnum;        // Program header table entry count
    uint16_t  e_shentsize;    // Section header table entry size
    uint16_t  e_shnum;        // Section header table entry count
    uint16_t  e_shstrndx;     // Section header string table index
}__attribute__((packed));

struct elf_program_header {
    uint32_t type;          // Segment type
    uint32_t offset;        // Offset of segment within the image
    uint32_t vaddr;       // Virtual address of the segment
    uint32_t paddr;         // Physical address of segment
    uint32_t filesz;        // Size of the segment in the file image
    uint32_t memsz;         // Size of the segment in memory
    uint32_t flags;         // Segment-dependent flags
    uint32_t align;         // 0/1: No alignment. Otherwise, should be power of 2
};

int parse_elf_header(void *start);

#endif


