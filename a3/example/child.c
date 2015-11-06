#include <stdio.h> 

int main(int argc, char *argv[]){
  int i = 0;
  printf("I am the child\n");
  while (--argc)
    printf ("Arg %d %s\n", ++i, *++argv);
  return 0;
}
