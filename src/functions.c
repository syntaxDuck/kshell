#include <stdlib.h>
#include <stdio.h>

#include "functions.h"


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
