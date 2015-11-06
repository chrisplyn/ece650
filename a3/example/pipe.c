#define STD_INPUT  0    /* file descriptor for standard input  */
#define STD_OUTPUT 1    /* file descriptor for standard output */
#define READ  0         /* read file descriptor from pipe  */
#define WRITE 1         /* write file descriptor from pipe */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

/* join two program by pipe */
int join( const char *prog1, char *const arg1[], const char *prog2, char *const arg2[])
{
    int p[2];       /* pipe */
    int status;     /* status */

    /* create child to run commands */
    switch( fork() )
    {
        case -1:
            /* fork was unsuccessful */
            printf( "unable to fork!\n" );
            return( -1 );

        case 0:
            /* child process */
        //printf("from the first child (pid %d)\n", getpid());
        break;

        default:
            /* parent process */
            wait(&status);
            return(status);
    }

    /* remainder of routine executed by child */

    /* create pipe */
    if( pipe(p) < 0 ) /* can use 'p' since address */
    {
        printf("unable to pipe!\n");
        return(-1);
    }

    /* create another process */
    switch(fork())
    {
        case -1:
            /* fork was unsuccessful */
            printf( "unable to fork!\n" );
            return( -1 );

        case 0:
            /* child process */
            close( STD_OUTPUT );    /* close standard output */

            /* make standard output go to pipe */
            dup( p[WRITE] );

            close( p[READ] );       /* close file descriptors */
            close( p[WRITE] );

            /* execute program 1 */
            execv(prog1, arg1);

            /* if execvp returns, error occured */
            printf( "first execv call failed!\n" );
            return( -1 );

        default:
            /* parent process */
            close( STD_INPUT );     /* close standard input */

            /* make standard input come from pipe */
            dup( p[READ] );

            close( p[READ] );       /* close file descriptors */
            close( p[WRITE] );

            /* execute program 2 */
            execv(prog2, arg2);

            /* if execvp returns, error occured */
            printf( "second execv call failed!\n" );
            return( -1 );
    }
}

int main( int argc, char *argv[] )
{
    char *arg1[2] = {"prog1.exe", 0 };
    char *arg2[2] = {"prog2.exe", 0 };
    int rval;   /* return value */

    rval = join("prog1.exe",arg1,"prog2.exe",arg2);
    printf("join returned %d\n", rval);
    return 0;
}