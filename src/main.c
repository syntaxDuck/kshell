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

#include "functions.h"

#define FALSE 0
#define TRUE 1

#define MAX_PATH_SIZE 4096 // max number of characters in a path
#define MAX_COM 1000       // max number of characters in a command
#define MAX_LIST 100       // max number of commmands to cache

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

char **getCommand(char *input) {

  // Create space for 8 tokens //
  char **command = createCharArray(8);
  char *delim = " ";
  char *token;
  int index = 0;

  if (command == NULL) {
    perror("Melloc Failed on command allocation\n");
    exit(1);
  }

  token = strtok(input, delim);

  while (token != NULL) {
    command[index] = malloc(strlen(token) + 1);
    strcpy(command[index], token);
    index++;

    token = strtok(NULL, delim);
  }

  command[index] = NULL;

  return command;
}

char *getCWD() {

  char *cwd = NULL;
  int size = MAX_PATH_SIZE;
  int maxTries = 5;

  do {
    maxTries--;
    cwd = (char *)realloc(cwd, size);

    if (cwd == NULL) {
      perror("Failed to allocate memory for current working directory");
      exit(1);
    }

    if (getcwd(cwd, size) == NULL) {
      if (errno == ERANGE) {
        // Checks that the buffer was too small and soubles it to try agian
        size *= 2;
        if (size > MAX_PATH_SIZE) {
          printf("Failed to get current working directory in elloted MAX_PATH_SIZE: %d\n",MAX_PATH_SIZE);
          return NULL;
        }
      } else {
        perror("Failed to get current working directory");
        free(cwd);
        return NULL;
      }
    } else {
      // Succefully obtained the current working directory
      return cwd;
    }
  } while (maxTries > 0);
  
  // Failed to obtain current working directory
  return NULL;
}

void change_directory(char *path) {

  if (path == NULL) return;

  if (chdir(path) < 0) {
    perror("Failed to change directory");
    return;
  } else return;
}

int main() {

  char **command = NULL;
  char *input = NULL;
  char *cwd = NULL;
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

        if (!strcmp(command[0], "cd")) {
          change_directory(command[1]);
        }

        else if (execvp(command[0], command) < 0) {
          perror("Failed to execute command through execvp call\n");
          exit(1);
        }
      } else {
        waitpid(child_pid, &child_status, WUNTRACED);
      }
    } else {
      return 1;
    }

    freeCharArray(command);
    free(input);
    free(cwd);
  }

  return 0;
}
