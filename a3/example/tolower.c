#include <ctype.h>
#include <stdio.h> 
#include <stdlib.h> 

void err_sys(const char* x) { 
    perror(x); 
    exit(1); 
}

int main(void)
{
    int c;

    while ((c = getchar()) != EOF) {
        if (isupper(c))
            c = tolower(c);
        if (putchar(c) == EOF)
            err_sys("output error");
        if (c == '\n')
            fflush(stdout);
    }
    exit(0);
}
