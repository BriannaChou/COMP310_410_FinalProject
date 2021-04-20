
#include "uart.h"
#include "serial.h"

#define MU_IO_ADR 0x3F215040

void putc(int data) {
	unsigned int * mu_io_reg = (unsigned int *) MU_IO_ADR;
	*mu_io_reg = data;
}

void readLine(char *input) {
	int c_counter = 0;
	clear_array(input, MAX_INPUT_SIZE);
	char c;
	// read until ENTER key is pressed
	while((c = uart_getc()) != '\n') {
		// print character to shell
		uart_send(c);
		input[c_counter] = c;
		c_counter++;
	}
	uart_send(c);
	// end line with linebreak
	input[c_counter] = '\n';
}

// clears array of given length
void clear_array(char *array, int array_size) {
	for(int i = 0; i < array_size; i++) {
		array[i] = 0;
	}
}
