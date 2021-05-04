#include "uart.h"
#include "serial.h"
#include "rprintf.h"

int main(int argc, char *argv[]) {

	char input_command[MAX_INPUT_SIZE];

	while(1) {
		// 1. Print command line prompt
		esp_printf((void *) putc, "$ ");
		
		// 2. Read input line
		// 	readLine function requires an argument (char *) that it can fill
		// 	readLine function is also responsible for for writing received char from keyboard to terminal
	   	readLine(input_command);
		if(input_command == NULL) {
			esp_printf((void *) putc, "Error while reading console. Exit code 1\n");
		}

		// 3. Print received command to terminal
	   	esp_printf((void *) putc, input_command);
   	}
}

