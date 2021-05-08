
// A lot of info taken from:
// http://www.independent-software.com/operating-system-development-file-allocation-table-and-reading-from-disk.html


#include "sd.h"
#include "nfat.h"
#include "rprintf.h"
#include <string.h>
#include "clibfuncs.h"

#define SECTOR_SIZE 32

unsigned int root_sector;
struct boot_sector bootsector;
unsigned char fat_table[8*SECTOR_SIZE]; // HACK ALERT!!! ASSUMING ONLY 8 SECTORS IN THE FAT. THIS SHOULD BE DYNAMICALLY ALLOC'D

/*
 * readFAT
 *
 * Reads the FAT into memory.
 */
static void readFAT(struct boot_sector *bs){
    // FAT starts right after the reserved sectors.
    sd_readblock(bs->num_reserved_sectors, fat_table, 8);
}

/*
 * fatFSInit
 *
 * Initializes the FAT driver. Reads boot sector from disk and stores it in
 * memory. This will be needed by other functions later. Also reads the FAT.
 */
int fatFSInit(){
    unsigned char blockBuf[CLUSTER_SIZE];
    struct boot_sector *bs = (struct boot_sector*)blockBuf;
    char oem_name[10], vol_label[12], fs_type[10];

    sd_readblock(0, blockBuf, 1);

    memset(oem_name, 0, sizeof(oem_name));
    memset(vol_label, 0, sizeof(vol_label));
    memset(fs_type, 0, sizeof(fs_type));

    memcpy(oem_name, bs->oem_name, 8);
    memcpy(vol_label, bs->volume_label, 11);
    memcpy(fs_type, bs->fs_type, 8);

    // Validate boot signature
    if(bs->boot_signature != 0xaa55) {
        return -1;
    }

    // Validate FS Type
    if(strncmp(fs_type, "FAT16", 5) != 0) {
        return -2;
    }

    readFAT(bs); // Read the volume's FAT into main memory

    memcpy(&bootsector, blockBuf, sizeof(struct boot_sector)); // Store boot sector for future use
    root_sector = bs->num_fat_tables * bs->num_sectors_per_fat + bs->num_reserved_sectors + bs->num_hidden_sectors;

    return 0;
}


/*
 * fatClose()
 *
 * Closes the file fd
 */
int fatClose(struct file *fd) {
    // TODO: Implement this...
    return 0;
}

/*
 * Opens a file. Only one file can be open at a time.
 *
 *
 */
int fatOpen(char *path, struct file *fd) {
    unsigned char blockBuf[bootsector.bytes_per_sector*bootsector.num_sectors_per_cluster];
    struct root_directory_entry *rde;
    unsigned int j,k, direntry_cluster_offset = 0;
    char filename[15], upperpath[15], dirname[15], *pathptr; // HACK ALERT!! SHOULD DYNAMICALLY ALLOCATE SPACE FOR THESE

    // Convert path to uppercase. All paths are stored in uppercase.
    strcpy(upperpath,path);
    k = 0;
    while(upperpath[k] != '\0'){
        upperpath[k] = toupper(path[k]);
        k++;
    }
    pathptr = upperpath + 1; // point pathptr past the initial '/' character in the path
    int i = 0;
    while(1) {
        strcpy(dirname,pathptr); // Copy the end of the path into dirname
        while((dirname[i] != '/') && (dirname[i] != '\0')){ // Find the end of the directory name in the path
            i++;
        }
        dirname[i] = '\0';
        for(k = 0; k < bootsector.num_root_dir_entries; k++){
            if((k*sizeof(struct root_directory_entry)) % (SECTOR_SIZE) == 0) {
                // When we cross a sector boundary, read in the next sector
                // If this is not the root directory (located at root_sector), read in starting at the direntry_cluster_offset. 
                if(sd_readblock(root_sector+direntry_cluster_offset*bootsector.num_sectors_per_cluster+(k*sizeof(struct root_directory_entry)/SECTOR_SIZE), blockBuf, 1) < 0) {
                    return -1;
                }
            }
            memset(filename,0,sizeof(filename));
            rde = (struct root_directory_entry*)((uint8_t*)blockBuf + (k*sizeof(struct root_directory_entry)));
            j = 0;
            while((j < 8) && (rde->file_name[j] != ' ')) {
                filename[j] = rde->file_name[j];
                j++;
            }
            // Append extension to file name if extension not empty
            if(rde->file_extension[0] != '\0' && rde->file_extension[0] != ' ') {
                filename[j++] = '.';
                memcpy(&filename[j], rde->file_extension,3);
            }
            if(filename[0] != '\xe5' && filename[0] != '\0') { // 0xe5 is free entry marker
                if(strcmp(filename,dirname) == 0) { // Found matching filename!
                    if(rde->attribute & FILE_ATTRIBUTE_SUBDIRECTORY) { // Found a subdirectory in the path
                        direntry_cluster_offset = rde->cluster; // Set direntry cluster offset, which points to the cluster of the subdirectory
                        pathptr = upperpath + i + 2;
                        break; // Break out of this loop and continue parsing the path.
                    }
                    fd->start_cluster = rde->cluster; // Store the starting cluster in the open file struct
                    memcpy(&fd->rde, rde, sizeof(struct root_directory_entry)); // Store file's RDE
                    return 0;
                }
            }
        }
    }
    return -1;
}

/*
 * readFAT12Entry
 *
 * Reads the file allocation table entry at index. We can't just index directly
 * into the FAT because each entry in a FAT12 takes 1.5 bytes.
 *
 */
static unsigned int readFAT12Entry(unsigned int index) {

    unsigned int nibbleNum = index * 3;
    unsigned int startingByteNum = nibbleNum / 2;

    if(nibbleNum & 1) { // nibbleNum odd
        return  (*(uint16_t*)(fat_table + startingByteNum) >> 4) & 0xfff;
    } else { // nibbleNum even
        return  *(uint16_t*)(fat_table + startingByteNum) & 0xfff;
    }
}


/*
 * fatRead
 *
 * Read count bytes from fd into buf.
 */
int fatRead(struct file *fd, unsigned char *buf, unsigned int count) {
    unsigned int fatEntry = fd->start_cluster;
    unsigned int n = 0;
    unsigned char clusterBuffer[bootsector.bytes_per_sector*bootsector.num_sectors_per_cluster];
    unsigned int fatBaseSector = bootsector.num_reserved_sectors + bootsector.num_hidden_sectors + bootsector.num_sectors_per_fat * bootsector.num_fat_tables + bootsector.num_root_dir_entries*32/512;

    count = count > fd->rde.file_size ? fd->rde.file_size : count; // Count only as big as file size
    
    while(n < count){ // Copy file into buf
        sd_readblock(fatBaseSector + (fatEntry-2)*bootsector.num_sectors_per_cluster, clusterBuffer, bootsector.num_sectors_per_cluster); // Not sure why we need to subtract 2 from the fatEntry to get its address on disk.
        if(count-n < bootsector.bytes_per_sector*bootsector.num_sectors_per_cluster){
            memcpy(buf+n, clusterBuffer, count-n);
            n += count-n;
            break;
        } else {
            memcpy(buf+n, clusterBuffer, bootsector.bytes_per_sector*bootsector.num_sectors_per_cluster);
            n += bootsector.bytes_per_sector*bootsector.num_sectors_per_cluster;
        }
        fatEntry = readFAT12Entry(fatEntry);
        if(fatEntry >= 0xff8){ // FAT Entry above ff8 indicates end of file
            break;
        }
    }
    return count;
}

/*
 * fatGetFileSize
 *
 * Returns the size of the currently open file.
 */
unsigned int fatGetFileSize(struct file *fd){
    return fd->rde.file_size;
}

