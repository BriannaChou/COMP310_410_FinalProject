
#include "uart.h"
#include "rprintf.h"
#include "serial.h"
#include "shell.h"

#define MAX_INPUT_SIZE 1000

void kernel_main(int argc, char *argv[]) {

	shell_main(argc, argv);
}
