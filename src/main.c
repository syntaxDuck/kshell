#include <readline/history.h>
#include <readline/readline.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commmands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

char **get_command(char *input) {
  char **command = malloc(8 * sizeof(char *));
  char *delim = " ";
  char *token;
  int index = 0;

  if (command == NULL) {
    perror("Melloc Failed on command allocation\n");
    exit(1);
  }

  token = strtok(input, delim);

  while (token != NULL) {
    command[index] = token;
    index++;

    token = strtok(NULL, delim);
  }

  command[index] = NULL;

  return command;
}

int main() {

  char **command;
  char *input;
  char *cwd;
  pid_t child_pid;
  int child_status;

  while (1) {
    if (getcwd(cwd, MAXCOM) != NULL) {
      printf("%s", cwd);
      input = readline("> ");
      command = get_command(input);

      child_pid = fork();
      if (child_pid < 0) {
        perror("Failed creating child process\n");
        exit(1);
      }

      if (child_pid == 0) {
        if (execvp(command[0], command) < 0) {
          perror("Command execution failed\n");
          exit(1);
        }
      } else {
        waitpid(child_pid, &child_status, WUNTRACED);
      }

    } else {
      perror("getcwd() error");
      return 1;
    }

    free(command);
    free(input);
    free(cwd);
  }

  return 0;
}
