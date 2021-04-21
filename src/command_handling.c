//File for command handling.
//Read input command with getLine() and then compare with existing commands.

//Get the return from getLine()

#include "fat.h"
#include "rprintf.h"

char testStr[] = "cd";

void checkCommand(char testStr){
	if( testStr == "cd"){
		//Change Directory
	}
	else if( testStr == "ls"){
		//list Directory
		esp_printf(putc, "THIS IS LISTING FILES");
	}
	else if( testStr == "cp"){
		//Copy file/folder
	}
	else if( testStr == "mkdir"){
		//Make Directory
	}
	else if( testStr == "rmdir"){
		//Remove Directory
	}
	else (testStr == "mv"){
		//Move file/folder
	}
}
