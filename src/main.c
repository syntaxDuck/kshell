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

#define RED_TXT_COLOR() printf("\x1b[31m");
#define GREEN_TXT_COLOR() printf("\x1b[32m");
#define BLUE_TXT_COLOR() printf("\x1b[34m");
#define YELLOW_TXT_COLOR() printf("\x1b[33m");
#define RESET_TXT_COLOR() printf("\x1b[0m");

void customPrint(char *string, enum TextColor color) {
  enum TextColor textColor;

  int str_len = strlen(string);

  switch (color) {
  case RESET:
    RESET_TXT_COLOR();
    break;
  case RED:
    RED_TXT_COLOR();
    break;
  case GREEN:
    GREEN_TXT_COLOR();
    break;
  case BLUE:
    BLUE_TXT_COLOR();
    break;
  case YELLOW:
    YELLOW_TXT_COLOR();
    break;
  }

  printf("%s", string);
  RESET_TXT_COLOR();
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
  char *host = getenv("HOST"); 
  
  printf("\x1b[32m%s@%s\n", user, host);
  using_history();

  // main loop
  while (1) {

    sprintf(prompt, "\x1b[32m%s>\x1b[0m ", cwd=getCWD());
    // check that cwd was aquired
    if (cwd != NULL) {
      // customPrint(strcat(cwd, "> "), GREEN);

      input = readline(prompt);

      if (strlen(input) > 0) {
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
