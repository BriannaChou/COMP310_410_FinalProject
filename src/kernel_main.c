
#include "uart.h"
#include "mmu.h"
#include "fat.h"
#include "nalloc.h"
#include "page.h"
#include "process.h"

#define MAX_INPUT_SIZE 1000

void kernel_main(int argc, char *argv[]) {

	uart_init();

	//init_pfa_list();

	memInit();

	fatInit();
	
	// ... Put your other init code here like MMU init etc

	exec("/bin/shell");
}
