#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "commands.h"
#include "functions.h"

#define FALSE 0
#define TRUE 1

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

extern struct CommandMapping commandMap[];
extern int commandMapSize;

int main() {

  struct Command command;
  char *input;
  char *cwd;
  pid_t child_pid;
  int child_status;

  while (1) {

    cwd = getCWD();
    if (cwd != NULL) {
      printf("%s", cwd);
      input = readline("> ");
      command = getCommand(input);

      child_pid = fork();
      if (child_pid < 0) {
        perror("Failed creating child process\n");
        exit(1);
      }

      if (child_pid == 0) {

        for (int i = 0; i < commandMapSize; i++) {
          if (strcmp(command.command[0], commandMap[i].name) == 0) {
            commandMap[i].functionPtr(command.command);
            command.executed = 1;
            break;
          }
        }

        if (!command.executed) {
          if (execvp(command.command[0], command.command) < 0) {
            perror("Failed to execute command through execvp call\n");
            exit(1);
          }
        }

      } else {
        waitpid(child_pid, &child_status, WUNTRACED);
      }
    } else {
      return 1;
    }

    freeCharArray(command.command);
    free(input);
    free(cwd);
  }

  return 0;
}
