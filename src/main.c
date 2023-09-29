#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/history.h>
#include <readline/readline.h>

#define FALSE	0
#define TRUE	1

#define MAXCOM  1000 // max number of letters to be supported
#define MAXLIST  100 // max number of commmands to be supported 

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int main() {

	char cwd[MAXCOM];
	char* test;

	clear();

	if(getcwd(cwd, sizeof(cwd)) != NULL) {

		test = readline(test);
		printf("Hello from me!");

	} else {
		perror("getcwd() error");
		return 1;
	}
	
	clear();
     	return 0;
}
