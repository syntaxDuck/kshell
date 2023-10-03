#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "functions.h"
#include "commands.h"



#define MAX_PATH_SIZE 4096 // max number of characters in a path
#define MAX_COM 1000       // max number of characters in a command
#define MAX_LIST 100       // max number of commmands to cache



char **createCharArray(int length) {
  char **array = malloc(length * sizeof(char*)); 
  return array;
}

void freeCharArray(char **array) {
  if (array == NULL) return;

  for (int i = 0; array[i] != NULL; i++) {
    free(array[i]);
    array[i] = NULL;
  
  }

  free(array);
}

struct Command getCommand(char *input) {

  struct Command command;
  // Create space for 8 tokens //
  command.command = createCharArray(8);
  command.executed = 0;
  char *delim = " ";
  char *token;
  int index = 0;

  if (command.command == NULL) {
    perror("Melloc Failed on command allocation\n");
    exit(1);
  }

  token = strtok(input, delim);

  while (token != NULL) {
    command.command[index] = malloc(strlen(token) + 1);
    strcpy(command.command[index], token);
    index++;

    token = strtok(NULL, delim);
  }

  command.command[index] = NULL;

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
