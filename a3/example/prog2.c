#include <stdio.h> 
#include <stdlib.h>

#define MAXLINE 1024

void err_sys(const char* x) { 
    perror(x); 
    exit(1); 
}

int main(int argc, char *argv[]){
  
  printf("The second program reads the output from the first program\n");
  
  int c;

    while ((c = getchar()) != EOF) {
        if (isupper(c))
            c = tolower(c);
        if (putchar(c) == EOF)
            err_sys("output error");
        if (c == '\n')
            fflush(stdout);
    }

  return 0;
}