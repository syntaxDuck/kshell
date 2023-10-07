#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "commands.h"

struct CommandMapping commandMap[] = {{"cd", changeDirectory}};

const int commandMapSize = sizeof(commandMap) / sizeof(commandMap[0]);


void changeDirectory(char **command) {
  if (command == NULL) return;

  if (chdir(command[1]) < 0) {
    perror("Failed to change directory");
    return;
  } else return;
}
