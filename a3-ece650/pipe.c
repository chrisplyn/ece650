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
void join(const char *prog1, char *const arg1[], const char *prog2, char *const arg2[],const char *prog3)
{
    int p1[2],p2[2],pid1,pid2;
    /* create child to run commands */
    if( pipe(p1) < 0 ) {
        fprintf(stderr, "Failed to create first pipe!\n");
        exit(1);
    }

    switch(pid1=fork())
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
            close(p1[WRITE]);       
            FILE *f1 = fdopen(p1[READ],"r");
            FILE *f2 = popen(prog3,"w");
            char buffer[10000];
            int count = 0;

            while (!feof (f1) && !ferror (f1) && fgets (buffer, sizeof(buffer), f1) != NULL) {
                count++;
                printf("%s",buffer);
                fflush(stdout);
                fprintf(f2, buffer);
                fflush(f2);

                if((count % 2) == 0) {
                    const int c = getc(stdin);  /*peek next char*/
                    
                    if(c == EOF){   /*sees EOF, terminate all process*/
                        close(p1[READ]);
                        pclose(f2);
                        exit(0);    
                    }
                    else {
                      ungetc(c,stdin);  
                    }

                    /*send input from stdin to a2-ece650*/
                    while(fgets(buffer,sizeof(buffer),stdin)!=NULL) {
                        fprintf(f2, buffer);
                        fflush(f2);
                    }
                }
            }       
    }

    /* create the second pipe */
    if(pipe(p2) < 0) {
        fprintf(stderr, "Failed to create the second pipe!\n");
        exit(1);
    }

    /* create child process of the child process */
    switch(pid2=fork()) {
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
            execvp(prog1, arg1);

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
            execvp(prog2, arg2);

            /* if execvp returns, error occured */
            fprintf(stderr, "Failed to execute the python script!\n");
            exit(1);
    }
}


/*check validity of command line argument */
void checkValidArg(int argc, char *argv[]){
    if(argc == 1) return;

    int i,s,n,l,c;    
    for(i = 1; i < argc; i+=2){
        if(strcmp(argv[i],"-s") == 0){
            s = atoi(argv[i+1]);

            if(s < 2 || s > 50){
                fprintf(stderr, "Number of streets is out of range!\n");
                exit(1);
            }
        }
        else if(strcmp(argv[i],"-n") == 0){
            n = atoi(argv[i+1]);

            if(n < 1 || n>50){
                fprintf(stderr, "Number of line segments in each street is out of range!\n");
                exit(1);
            }
        }
        else if(strcmp(argv[i],"-l") == 0){
            l = atoi(argv[i+1]);

            if(l < 5 || l>50){
                fprintf(stderr, "Number of waiting time is out of range!\n");
                exit(1);
            }
        }
        else if(strcmp(argv[i],"-c") == 0){            
            c = atoi(argv[i+1]);
            
            if(c < 1 || c > 50){
                fprintf(stderr, "Invalid coordinate range!\n");
                exit(1);
            }
        }
        else{
            fprintf(stderr, "Invalid command line argument!\n");
            exit(1);
        }
    }
}



int main( int argc, char *argv[]) {
    checkValidArg(argc,argv);

    char *rgenArg[argc];
    rgenArg[0] = "rgen";
    int k;

    for(k=1; k<argc; ++k) {
        rgenArg[k] = argv[k];
    }
    rgenArg[argc] = 0;  /*null terminated*/

    char *pythonArg[2] = {"a1-ece650.py", 0};
    join("./rgen",rgenArg,"./a1-ece650.py",pythonArg,"./a2-ece650");
    return 0;
}