#include <errno.h>
#include <readline/chardefs.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "commands.h"
#include "functions.h"

extern struct CommandMapping commandMap[];
extern int commandMapSize;

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Text modifiers //
enum TextColor { RESET, RED, GREEN, BLUE, YELLOW };

#define RED_TXT "\x1b[31m"
#define GREEN_TXT "\x1b[32m"
#define BLUE_TXT "\x1b[34m"
#define YELLOW_TXT "\x1b[33m"
#define RESET_TXT "\x1b[0m"

void customPrint(char *string, enum TextColor color) {
  enum TextColor textColor;

  int str_len = strlen(string);
  char escape_seq[9];


  switch (color) {
  case RESET: 
    printf(RESET_TXT "%s", string );
    break;
  case RED:
    printf(RED_TXT "%s", string );
    break;
  case GREEN:
    printf(GREEN_TXT "%s", string );
    break;
  case BLUE:
    printf(BLUE_TXT "%s", string );
    break;
  case YELLOW:
    printf(YELLOW_TXT "%s", string );
    break;
  }
  printf(RESET_TXT);
}

void procInput(char *input) {
  struct Command command;
  pid_t child_pid;
  int child_status;

  command = getCommand(input);

  child_pid = fork();

  // check if child process was created successfully
  if (child_pid < 0) {
    perror("Failed creating child process\n");
    exit(1);
  }

  // check if current process is parent or child
  // child process won't have a child_pid where the parent process will
  if (child_pid == 0) {

    // iterate over commands in map
    for (int i = 0; i < commandMapSize; i++) {
      if (strcmp(command.command[0], commandMap[i].name) == 0) {
        commandMap[i].functionPtr(command.command);
        command.executed = true;
        break;
      }
    }

    // if commands was not in map attempt to execute from existing bin
    if (!command.executed) {
      if (execvp(command.command[0], command.command) < 0) {
        perror("Failed to execute command through execvp call\n");
        exit(EXIT_FAILURE);
      } else {
        command.executed = true;
      }
    }

  } else {
    // Have parent process to wait for child process to finish
    waitpid(child_pid, &child_status, WUNTRACED);
  }

  freeCharArray(command.command);
}

int main() {

  char *input;
  char *cwd;
  char prompt[4096];
  char *user = getenv("USER");
  char host[128];
  gethostname(host, sizeof(host));
  
  printf(GREEN_TXT "%s@%s\n" RESET_TXT, user, host);
  using_history();

  // main loop
  while (1) {

    sprintf(prompt, GREEN_TXT "%s> " RESET_TXT, cwd=getCWD());
    // check that cwd was aquired
    if (cwd != NULL) {
      input = readline(prompt);

      if (strlen(input) > 0) {
        add_history(input);
        procInput(input);
      }
    } else {
      exit(EXIT_FAILURE);
    }

    free(input);
    free(cwd);
  }

  exit(EXIT_SUCCESS);
}
