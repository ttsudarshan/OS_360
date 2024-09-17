#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
int main() { 
  for (int i=0; i< 3; i++){
     fork();
  }
  printf("hello\n");
  return 0;
}