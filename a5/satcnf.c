#include "satcnf.h"
#include "SAT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

void* sat_cnf(void *parameters) {
	int  k = 1;// minimum vertex cover
	listNode* cur;
	thread_function_args *param = (thread_function_args *)parameters;
	int numNodes = param->numNodes;
	list *edgeList = param->edgeList;
	struct timespec start, end;

	if(param->vc == NULL) {
		param->vc = (int *)malloc(numNodes*sizeof(int));
	}
	
	int p[2], bak;
	if(pipe(p) < 0){
		fprintf(stderr, "Error: pipe failed\n");
		exit(-1);
	}
	bak = dup(1);
    dup2(p[1], 1);

	clockid_t cid;
	pthread_getcpuclockid(pthread_self(), &cid);
	clock_gettime(cid, &start);

	while(k <= numNodes){	
		int i,j,p,q;
       	int val = 1; /* value of variable, starts from 1*/
		int x[numNodes][k]; 
		// memset(vc, 0, numNodes*sizeof(int));

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
		cur = edgeList->head;

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

		if(result == SATISFIABLE) {
			int index = 0;
			int n = SAT_NumVariables(mgr);

			for(j = 1; j <= n; j++) {
				int a = SAT_GetVarAsgnment(mgr, j);

				if(a == 1) {
					if(j <= numNodes) {
						param->vc[index] = j - 1;
					}
					else {
						if(j % numNodes == 0) {
							param->vc[index] = n - 1;
						}
						else {
							param->vc[index] = j % numNodes - 1;
						}
					}

					index++;
				}
			}

			qsort(param->vc, index, sizeof(int), compare);
			param->vcSize = index;
			SAT_ReleaseManager(mgr); 
			dup2(bak, 1);                 
			break;
		}   

		k++;
		SAT_ReleaseManager(mgr);
	}

	pthread_getcpuclockid(pthread_self(), &cid);
	clock_gettime(cid, &end);
	param->cputime = timediff(&start,&end);
}