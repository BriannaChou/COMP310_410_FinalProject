
#include "uart.h"
#include "mmu.h"
#include "fat.h"
#include "nalloc.h"
#include "page.h"
#include "process.h"
#include "rprintf.h"
#include "serial.h"

#define MAX_INPUT_SIZE 1000

void kernel_main(int argc, char *argv[]) {

	uart_init();

	//init_pfa_list();

	memInit();

	fatInit();

	esp_printf((void *) putc, "3. FAT FS initialized!");
	
	// ... Put your other init code here like MMU init etc

	exec("/bin/shell");
}
