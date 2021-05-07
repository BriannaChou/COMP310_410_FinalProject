
#include "uart.h"
#include "mmu.h"
#include "fat.h"
#include "nalloc.h"
#include "page.h"
#include "process.h"
#include "rprintf.h"
#include "serial.h"
#include "sd.h"

#define MAX_INPUT_SIZE 1000

void kernel_main(int argc, char *argv[]) {
	
	// Initializations
	uart_init();

	//init_pfa_list();
	
	memInit();

	sd_init();

	fatInit();

	exec("/shell");
}
