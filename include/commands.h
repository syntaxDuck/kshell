#ifndef COMMANDS_H
#define COMMANDS_H

struct Command {
  char **command;
  int executed;
};

struct CommandMapping {
  const char *name;
  void (*functionPtr)(char **);
};

void changeDirectory(char **command);

#endif
