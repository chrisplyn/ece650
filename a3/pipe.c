#define STD_INPUT  0    /* file descriptor for standard input  */
#define STD_OUTPUT 1    /* file descriptor for standard output */
#define READ  0         /* read file descriptor from pipe  */
#define WRITE 1         /* write file descriptor from pipe */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/* join three programs by pipe */
void join(const char *prog1, char *const arg1[], const char *prog2, char *const arg2[],const char *prog3, char *const arg3[])
{
    int p1[2],p2[2];       /* pipe */
    int status;     /* status */

    /* create child to run commands */
    if( pipe(p1) < 0 ) {
        fprintf(stderr, "Failed to create first pipe!\n");
        exit(1);
    }

    switch(fork())
    {
        case -1:
            /* fork was unsuccessful */
            fprintf(stderr, "Failed to create first child process!\n");
            exit(1);
        case 0:
            /* child process */
            close(STD_OUTPUT);

            /* make standard outputs of python script go to pipe */
            /* and close file descriptors */
            dup(p1[WRITE]);
            close(p1[READ]);
            close(p1[WRITE]);
            break;
        default:
            /* parent process */
            close(STD_INPUT);

            /* make standard input of the a2-ece650 come from pipe, i.e., the output of python script*/
            /* and close file descriptors */
            dup(p1[READ]);
            close(p1[READ]);
            close(p1[WRITE]);
            wait(&status);

            /* execute program a2-ece650 */
            execv(prog3, arg3);  

            /*on success, execv does not return*/
            /*oterwise execv returns*/
            fprintf(stderr, "Failed to execute a2-ece650!\n");
            break;
    }

    /* create the second pipe */
    if(pipe(p2) < 0) {
        fprintf(stderr, "Failed to create the second pipe!\n");
        exit(1);
    }

    /* create child process of the child process */
    switch(fork()) {
        case -1:
            /* fork was unsuccessful */
            fprintf(stderr, "Failed to create a child process of the child process!\n");
            exit(1);

        case 0:
            /* child process of the child process*/
            /* close standard output first*/
            close(STD_OUTPUT);    

            /* make standard outputs of rgen go to pipe */
            /* and close file descriptors in that process*/
            dup(p2[WRITE]);
            close(p2[READ]);       
            close(p2[WRITE]);

            /* execute program rgen */
            execv(prog1, arg1);

            /* if execv returns, error occured */
            fprintf(stderr, "Failed to execute rgen!\n");
            exit(1);

        default:
            /* child process */
            /* close standard input */
            close(STD_INPUT);     
            
            /* make standard input of the python script come from pipe, i.e., the output of rgen*/
            /* and close file descriptors */
            dup(p2[READ]);
            close(p2[READ]);       
            close(p2[WRITE]);

            /* execute python script*/
            execv(prog2, arg2);

            /* if execvp returns, error occured */
            fprintf(stderr, "Failed to execute the python script!\n");
            exit(1);
    }
}

int main( int argc, char *argv[]) {
    char *arg1[2] = {"rgen", 0 };
    char *arg2[2] = {"a1-ece650.py", 0 };
    char *arg3[3] = {"a2-ece650", 0 };
    join("rgen",arg1,"a1-ece650.py",arg2,"a2-ece650",arg3);
    return 0;
}