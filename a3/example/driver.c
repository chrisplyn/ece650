#include <sys/wait.h>
#include <stdio.h> 
#include <stdlib.h> 

#define MAXLINE 1024

void err_sys(const char* x) { 
    perror(x); 
    exit(1); 
}


int main(void){
    char    line[MAXLINE];
    FILE    *fpin;

    if ((fpin = popen("./tolower.exe", "r")) == NULL)
        err_sys("popen error");
    for ( ; ; ) {
        fputs("prompt> ", stdout);
        fflush(stdout);
        if (fgets(line, MAXLINE, fpin) == NULL) /* read from pipe */
            break;
        if (fputs(line, stdout) == EOF) /*write to stdout*/
            err_sys("fputs error to pipe");
    }
    if (pclose(fpin) == -1)
        err_sys("pclose error");
    putchar('\n');
    exit(0);
}