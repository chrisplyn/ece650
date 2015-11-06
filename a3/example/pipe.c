#define STD_INPUT  0    /* file descriptor for standard input  */
#define STD_OUTPUT 1    /* file descriptor for standard output */
#define READ  0         /* read file descriptor from pipe  */
#define WRITE 1         /* write file descriptor from pipe */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

/* join two commands by pipe */
int join( char *com1[], char *com2[] )
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
        printf("from the first child (pid %d)\n", getpid());
        break;

        default:
            /* parent process */
            wait( &status );
            printf("from the parent (pid %d)\n", getpid());
            return( status );
    }

    /* remainder of routine executed by child */

    /* create pipe */
    if( pipe( p ) < 0 ) /* can use 'p' since address */
    {
        printf( "unable to pipe!\n" );
        return( -1 );
    }

    /* create another process */
    switch( fork() )
    {
        case -1:
            /* fork was unsuccessful */
            printf( "unable to fork!\n" );
            return( -1 );

        case 0:
            /* child process */
            printf("from the second child (pid %d)\n", getpid());
            close( STD_OUTPUT );    /* close standard output */

            /* make standard output go to pipe */
            dup( p[WRITE] );

            close( p[READ] );       /* close file descriptors */
            close( p[WRITE] );

            /* execute command 1 */
            execvp( com1[0], com1 );

            /* if execvp returns, error occured */
            printf( "first execvp call failed!\n" );
            return( -1 );

        default:
            /* parent process */
            printf("from the first child (pid %d)\n", getpid());
            close( STD_INPUT );     /* close standard input */

            /* make standard input come from pipe */
            dup( p[READ] );

            close( p[READ] );       /* close file descriptors */
            close( p[WRITE] );

            /* execute command 2 */
            execvp( com2[0], com2 );

            /* if execvp returns, error occured */
            printf( "second execvp call failed!\n" );
            return( -1 );
    }
}

int main( int argc, char *argv[] )
{
    char *one[4];   /* command 1 */
    char *two[3];   /* command 2 */
    int rval;   /* return value */

    /* list contents of /usr/lib */
    one[0] = "ls";
    one[1] = "-l";
    one[2] = "/usr/lib";
    one[3] = (char *)0;

    /* grep for directories */
    two[0] = "grep";
    two[1] = "^d";
    two[2] = (char *) 0;

    /* join command 1 and command 2 by pipe */
    rval = join( one, two );

    printf( "join returned %d\n", rval );

    return( 0 );
}