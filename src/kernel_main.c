
#include "uart.h"
#include "rprintf.h"
#include "serial.h"

#define MAX_INPUT_SIZE 1000

void kernel_main(int argc, char *argv[]) {

    uart_init();

    // ... Put your other init code here like MMU init etc

    //exec("/bin/shell");
}
