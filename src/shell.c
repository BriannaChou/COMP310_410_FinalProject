
#include "uart.h"
#include "rprintf.h"
#include "serial.h"

#define MAX_INPUT_SIZE 1000

int shell_main(int argc, char **argv) {

    uart_init();
    char input_command[MAX_INPUT_SIZE];

    while(1){

        // 1. Print command line prompt
        esp_printf((void *) putc, "$ ");

        // 2. Call readLine() to read a line from the terminal. The parsed argument (char *) is the data structure that will be filled with the terminal line
        //    readLine() is also responsible for writing every received key to the console so the user can read what he typed before rpessing Enter.
	//    TODO: Execute received command (either directory operation or process call)
	//    Currently: Echos received line back to terminal
	readLine(input_command);
	if(input_command == NULL) {
		esp_printf((void *) putc, "Error while reading console. Exit code 1\n");
	}
	uart_puts(input_command);

        // 3. getline() will return a line of text, including the \n newline
        //    character. We need to trim that off. Something like
        //           line[strlen(line)-1] = '\0';
        //    should do.
	//command[strlen(command)-1] = '\0';

	// Call command handling function here and pass parameter command


        // 4. Call fork():
        //           pid = fork();
        //           if(pid == 0) { // Child process
        //              ...
        //           } else {       // Parent process
        //              ...
        //           }

        // 5. For the child process, create an argument vector for the child process. This is needed by execv(). You can do this in a couple of ways:
        //    (i)  Create a fixed-size array of character pointers (char *argz[2]) as a local variable.
        //    (ii) Use malloc to allocate space for two character pointers (char *argz = malloc(2*sizeof(char*))).
        //    Either way, initialize argz[0] = line and argz[1] = NULL.
        //    Normally, each element in the argument vector would be a pointer
        //    to an argument passed on the command line. For example, if the
        //    user types the following command:
        //
        //    $ gcc -o shittyshell shittyshell.c
        //
        //    argv[0] = "gcc"
        //    argv[1] = "-o"
        //    argv[2] = "shittyshell"
        //    argv[3] = "shittyshell.c"
        //    argv[4] = NULL (Last element of argv always has to be NULL)
        //
        //    But since this is a shitty shell, you don't need to parse all the
        //    arguments into separate strings. Just make argz[0] point to the
        //    command the user typed, and you're done.


        // 6. For the child process, call execv():
        //          if(execv(line,argz) == -1) perror("nsh");
        //    If execv() executes successfully, it will replace the child's
        //    process image with the new process image, and it won't return.
        //    If there is an error, execv() will return with an error code
        //    (which will be printed by perror()).

        // 7. For the parent process, free(argz) and free(line), and go on to
        //    the next iteration of the loop.
    }
}

