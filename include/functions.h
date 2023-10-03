#ifndef FUNCTIONS_H
#define FUNCTIONS_H

char **createCharArray(int length);
void freeCharArray(char **array);
struct Command getCommand(char *input);
char *getCWD();

#endif
