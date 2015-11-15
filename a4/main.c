/*
 * main.c
 *
 *      Author: Yinuo Liu
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "SAT.h"

typedef struct{
  int p1,p2;
} edge;

int compare(const void*a, const void*b){
   return  *(int*)a-*(int*)b;
}

int main(int argc, char *argv[]) {
   char line,c;
   int numNodes, e1, e2;
   listNode* cur;
   list edgeList;
   edge e;
   createList(&edgeList, sizeof(edge), NULL);

   loop:while(scanf(" %c", &line) != EOF) {
      switch(line) {
         case 'V':
            scanf(" %d", &numNodes);

            if(numNodes < 0) {
               printf("Error: Invalid number of vertices: %d!\n", numNodes);
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
               scanf(" <%d,%d>", &e1,&e2);

               if( (e1 >= numNodes || e1 < 0) || (e2 >= numNodes || e2 < 0)) {
                  printf("Error: Invalid edge <%d,%d>!\n", e1, e2);
                  destroy(&edgeList);
                  goto loop;
               }

               e.p1 = e1;
               e.p2 = e2;
               append(&edgeList,&e);
               scanf("%c", &c); //scan ',' or '}'
            }

            int  k = 1;// minimum vertex cover         

            while(k <= numNodes){
               int pipe1[2], bak;
               pipe(pipe1);
               bak = dup(STDOUT_FILENO);
               dup2(pipe1[1], STDOUT_FILENO);
            
               int i,j,p,q;
               int val = 1; /* value of variable, starts from 1*/
               int x[numNodes][k], vc[numNodes]; 
               memset(vc, 0, numNodes*sizeof(int));

               /* initialize the value of every literal*/
               for(j = 0; j < k; j++){
                  for(i = 0; i < numNodes; i++){
                     x[i][j] = val++;
                  }
               }
            
               SAT_Manager mgr = SAT_InitManager();
               SAT_SetNumVariables(mgr, numNodes*k);
               
               int c[numNodes*k]; /*at most n*k literals in one clause*/
               
               /*for each column, at least one of the n vertices is in VC i*/
               for(j = 0; j < k; j++){
                  for(i = 0; i < numNodes; i++){
                     c[i] = (x[i][j] << 1);
                  }
                  SAT_AddClause(mgr, c, numNodes);
               }

               /*for each row (k>1), one vertex can't be both p^{th} and q^{th} vertex in the VC*/
               if(k > 1){
                  for(i = 0; i < numNodes; i++){
                     for(q = 0; q < k ; q++){
                        for(p = 0; p < q; p++){
                           c[0] = (x[i][p] << 1) + 1;
                           c[1] = (x[i][q] << 1) + 1;
                           SAT_AddClause(mgr, c, 2);
                        }
                     }
                  }
               }

               /*for each column, one vertex in VC can't be both p^{th} and q^{th} vertex*/
               for(j = 0; j < k; j++){
                  for(q = 0; q < numNodes; q++){
                     for(p = 0; p < q; p++){
                        c[0] = (x[p][j] << 1) + 1;
                        c[1] = (x[q][j] << 1) + 1;
                        SAT_AddClause(mgr, c, 2);
                     }
                  }
               }
               
               /*for each edge, at least one endpoint is in VC*/
               cur = edgeList.head;

               while(cur != NULL) {
                  edge *e = (edge *) cur->data;
                  
                  for(j = 0; j < k; j++) {   /*first endpoint*/
                     c[j] = (x[e->p1][j] << 1);
                  }

                  int tmp = k;
                  for(j = 0; j < k; j++) {   /*second endpoint*/
                     c[tmp] = (x[e->p2][j] << 1);
                     tmp++;
                  }

                  SAT_AddClause(mgr, c, 2*k);
                  cur = cur->next;
               }

               int result = SAT_Solve(mgr);
               dup2(bak, 1);

               if(result == SATISFIABLE) {
                  int index = 0;
                  int n = SAT_NumVariables(mgr);
                  int s;

                  for(j = 1; j <= n; j++) {
                     int a = SAT_GetVarAsgnment(mgr, j);

                     if(a == 1) {
                        if(j <= numNodes) {
                           vc[index] = j - 1;
                        }
                        else {
                           if(j % numNodes == 0) {
                              vc[index] = n - 1;
                           }
                           else {
                              vc[index] = j % numNodes - 1;
                           }
                        }

                        index++;
                     }
                  }

                  qsort(vc, index, sizeof(int), compare);

                  for(i = 0; i < index-1; i++){
                     printf("%d ", vc[i]);
                  }
                  printf("%d \n", vc[index-1]);
                  fflush(stdout);
                  SAT_ReleaseManager(mgr);
                  break;
               }   

               k++;
               SAT_ReleaseManager(mgr);
            }
         break;
      }
   }
   destroy(&edgeList);
}
