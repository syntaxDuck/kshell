#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>

#include "commands.h"
#include "functions.h"

char **createCharArray(int length) {
  char **array = malloc(length * sizeof(char *));
  return array;
}

void freeCharArray(char **array) {
  if (array == NULL)
    return;

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
          printf("Failed to get current working directory in elloted "
                 "MAX_PATH_SIZE: %d\n",
                 MAX_PATH_SIZE);
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

void customPrint(char *string, enum TextColor color) {
  enum TextColor textColor;

  int str_len = strlen(string);
  char escape_seq[9];

  switch (color) {
  case RESET:
    printf(RESET_TXT "%s", string);
    break;
  case RED:
    printf(RED_TXT "%s", string);
    break;
  case GREEN:
    printf(GREEN_TXT "%s", string);
    break;
  case BLUE:
    printf(BLUE_TXT "%s", string);
    break;
  case YELLOW:
    printf(YELLOW_TXT "%s", string);
    break;
  }
  printf(RESET_TXT);
}

int disableTermEchoBuff(struct TerminalSettings terminalSettings) {
  terminalSettings.newSettings.c_lflag &= ~(ICANON | ECHO);
  if (tcsetattr(terminalSettings.fd, TCSANOW, 
                &terminalSettings.newSettings) < 0) {
    perror("Failed to set terminal attributes");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int enableTermEchoBuff(struct TerminalSettings terminalSettings) {
  terminalSettings.newSettings.c_lflag |= (ICANON | ECHO);
  if (tcsetattr(terminalSettings.fd, TCSANOW, 
                &terminalSettings.newSettings) < 0) {
    perror("Failed to set terminal attributes");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int resetTermConfig(struct TerminalSettings terminalSettings) {
  if (tcsetattr(terminalSettings.fd, TCSANOW,
                &terminalSettings.originalSettings) < 0) {

    perror("Failed to set terminal attributes");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void getCursorPos(int *x, int *y) {
  printf("\033[6n");
  scanf("\033[%d;%dR", y, x);
}
