/*
 * main.c
 *
 *      Author: Yinuo Liu
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "satcnf.h"
#include "approx_1.h"
#include "approx_2.h"
#define N 2 

int numNodes = 0;
list edgeList;


int main(int argc, char *argv[]) {
   char line,c;
   int i,j,e1, e2;
   edge e;
   createList(&edgeList, sizeof(edge), NULL);
   pthread_t thread_satcnf, thread_approx_1, thread_approx_2;
      
   loop:while(scanf(" %c", &line) != EOF) {
      switch(line) {
         case 'V':
            scanf(" %d", &numNodes);

            if(numNodes < 0) {
               fprintf(stderr,"Error: Invalid number of vertices: %d!\n", numNodes);
               goto loop;
            }

            if(edgeList.length != 0) {
               destroy(&edgeList);             
            }

            break;
         case 'E':
            if(numNodes == 0) {  //empty graph??
               goto loop;
            }

            scanf(" %c", &c);

            while(c != '}') {
               if(!scanf(" <%d,%d>", &e1,&e2)) goto loop;

               if( (e1 >= numNodes || e1 < 0) || (e2 >= numNodes || e2 < 0)) {
                  fprintf(stderr,"Error: Invalid edge <%d,%d>!\n", e1, e2);
                  destroy(&edgeList);
                  goto loop;
               }

               e.p1 = e1;
               e.p2 = e2;
               append(&edgeList,&e);
               scanf("%c", &c); //scan ',' or '}'
            }

            thread_function_args thread_args[N];

            /*initialize parameters for each thread function*/
            for(i=0; i<N; i++) {
               thread_args[i].numNodes = numNodes;
               thread_args[i].edgeList = &edgeList;
               thread_args[i].vc = NULL;
            }

            int iter = 1;

            #ifdef DEBUG
               iter = 10;
               //double ratio1,ratio2;
               //double *runTimeSatCnf = (double *)malloc(iter*sizeof(double));
               double *runTimeApprox1 = (double *)malloc(iter*sizeof(double));
               double *runTimeApprox2 = (double *)malloc(iter*sizeof(double));
            #endif

            for(j=0; j<iter; j++) {
               //pthread_create(&thread_satcnf, NULL, &sat_cnf, &thread_args[0]);
               pthread_create(&thread_approx_1, NULL, &approx1, &thread_args[0]);
               pthread_create(&thread_approx_2, NULL, &approx2, &thread_args[1]);

               //pthread_join(thread_satcnf, NULL);
               pthread_join(thread_approx_1, NULL);
               pthread_join(thread_approx_2, NULL);   

               #ifdef DEBUG
                  //runTimeSatCnf[j] = thread_args[0].cputime;
                  runTimeApprox1[j] = thread_args[0].cputime;
                  runTimeApprox2[j] = thread_args[1].cputime;
               #endif
            }

            #ifdef DEBUG
               // ratio1 = thread_args[1].vcSize / (double) thread_args[0].vcSize;
               // ratio2 = thread_args[2].vcSize / (double) thread_args[0].vcSize; 

               for(j=0; j<iter; j++) {
                  printf("%f,%f\n", runTimeApprox1[j],runTimeApprox2[j]);
                  //printf("%f,%f,%f\n", runTimeSatCnf[j],runTimeApprox1[j],runTimeApprox2[j]);
                  fflush(stdout);
               }
               //printf("%f,%f\n", ratio1,ratio2);
               fflush(stdout);

               for(i=0; i<N; i++) {
                  free(thread_args[i].vc);
               }
               //free(runTimeSatCnf);
               free(runTimeApprox1);
               free(runTimeApprox2);
            #else
               const char *name[N] = {"CNF-SAT-VC", "APPROX-VC-1", "APPROX-VC-2"};
                  
               for(i=0; i<N; i++) {
                  printVC(thread_args[i].vcSize, thread_args[i].vc, name[i]);
                  free(thread_args[i].vc);
               }
            #endif

            break;   
      }
   }
   destroy(&edgeList);
}
