#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <termio.h>

#define MAX_PATH_SIZE 4096 // max number of characters in a path
#define MAX_COM 1000       // max number of characters in a command
#define MAX_LIST 100       // max number of commmands to cache

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

#define RED_TXT "\x1b[31m"
#define GREEN_TXT "\x1b[32m"
#define BLUE_TXT "\x1b[34m"
#define YELLOW_TXT "\x1b[33m"
#define RESET_TXT "\x1b[0m"

// Text modifiers //
enum TextColor { RESET, RED, GREEN, BLUE, YELLOW };

struct TerminalSettings {
  const int fd;
  struct termios newSettings;
  const struct termios originalSettings;
};

struct Command getCommand(char *input);

char **createCharArray(int length);
char *getCWD();

int disableTermEchoBuff(struct TerminalSettings terminalSettings);
int enableTermEchoBuff(struct TerminalSettings terminalSettings);
int resetTermCofig(struct TerminalSettings terminalSettings);

void freeCharArray(char **array);
void getCursorPos(int *x, int *y);

#endif
