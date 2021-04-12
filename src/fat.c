#include "sd.h" // See this file for function prototype of ata_lba_read()
#include "rprintf.h"
#include "fat.h"
#include "serial.h"
#include "clibfuncs.h"

struct boot_sector *bs;
char bootSector[512]; // Allocate a global array to store boot sector

struct root_directory_entry *fat_stuff;
char rootSector[512];

//char fat_table[8*SECTOR_SIZE];
unsigned int root_sector;
int fatInit() {
	char buffer[10];
	sd_readblock(0, bootSector, 1); // Read sector 0 from disk drive into bootSector array
	bs = bootSector; // Point boot_sector struct to the boot sector so we can read fields
	esp_printf(putc, "BOOT SETCTOR = %x\n", bs);
	int k = 0;
	while (bs->fs_type[k] != ' ' || k >= 8){
		esp_printf(putc,"%c", bs->fs_type[k]);
		buffer[k] = bs->fs_type[k];
		k++;
		buffer[k] = 0;
	}
	// Print out some of the elements of the BIOS information block using rprintf...
	if (bs->boot_signature == 0xaa55){// TODO: Validate the boot signature = 0xaa55
		esp_printf(putc, "Validated boot signature \n");
	}
	if (strcmp(buffer, "FAT16")){ // TODO: Validate fs_type = "FAT12" using strcmp
		 esp_printf(putc, "It is FAT16 \n");
	}
	esp_printf(putc,"Bytes per sector--> %d \n" ,bs->bytes_per_sector);
	esp_printf(putc,"Sectors per cluster--> %d \n" ,bs->num_sectors_per_cluster);
	esp_printf(putc,"Reserved Sectors--> %d \n" ,bs->num_reserved_sectors);
	esp_printf(putc,"FAT Tables--> %d \n" ,bs->num_fat_tables);
	esp_printf(putc,"Root Directory entries--> %d \n" ,bs->num_root_dir_entries);
	int root_sector_value = ((bs->num_fat_tables * bs->num_sectors_per_fat) + bs->num_hidden_sectors + bs->num_reserved_sectors);
	//root_sector_value = (bs->bytes_per_sector *root_sector_value);
	esp_printf(putc,"Root Sector value--> %d \n" ,root_sector_value);
}

struct file *file_open;
//Opens a file in a FAT filesystem on disk.
void fatOpen(struct file* whatever, char* filename){
	int t_sectors =  bs->num_root_dir_entries;
	int t = 1;
	char buffer[10];
	int root_sector_value = ((bs->num_fat_tables * bs->num_sectors_per_fat) + bs->num_hidden_sectors + bs->num_reserved_sectors);
	esp_printf(putc, "BOOT SETCTOR = %x\n", bs);
	sd_readblock(root_sector_value, rootSector, 1);
	fat_stuff = rootSector;
	while (t < t_sectors ){
		int k = 0;
		while (k < 8){
			esp_printf(putc,"%c", fat_stuff->file_name[k]);
			if( fat_stuff->file_name[k] != ' ')
				buffer[k] = fat_stuff->file_name[k];
			k++;
		}
		buffer[7] = 0;
		if (strcmp(buffer, filename) == 0){ // TODO: Validate fs_type = "FAT12" using strcmp
			esp_printf(putc, "Buffer is equal \n");
			whatever->rde = *fat_stuff;
			whatever->start_cluster = fat_stuff->cluster;
			return;
		}
		esp_printf(putc,"\n%s\n", buffer);
		t++;
		fat_stuff++;
	}
}

//Reads data from a file into a buffer
void fatRead(struct file* fileName,char buffer, int bytes_2_read){
	int root_sector_value = ((bs->num_fat_tables * bs->num_sectors_per_fat) + bs->num_hidden_sectors + bs->num_reserved_sectors);
	esp_printf(putc, "Cluster stuff--> %d \n", fileName->rde.cluster);
	int begin_data = bs->num_reserved_sectors + (bs->num_fat_tables * bs->num_sectors_per_fat) + ((bs->num_root_dir_entries * 32) + (bs->bytes_per_sector - 1)) / bs->bytes_per_sector;
	int data_sector = begin_data + ((fileName->rde.cluster-2) * bs->num_sectors_per_cluster);
	//int sd_readblock(unsigned int lba, unsigned char *buffer, unsigned int num);
	char buffer2[512];
	sd_readblock(data_sector,buffer2,1);
	esp_printf(putc,"Stuff-->%s",buffer2);
	int first_root_dir_sector = begin_data + ((bs->num_root_dir_entries)/512);
	esp_printf(putc, "First root dir --> %d \n", begin_data);
	esp_printf(putc, "variable y --> %d \n", data_sector);
}















