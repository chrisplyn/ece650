#include "approx_1.h"
#include <stdlib.h>

void* approx1(void *parameters) {	
	/*read thread function argument*/
	thread_function_args *param = (thread_function_args *)parameters;
	int numNodes = param->numNodes;
	list *edgeList = param->edgeList;
	
	if(param->vc == NULL) {
		param->vc = (int *)malloc(numNodes*sizeof(int));
	}

	/*initialize adjacency list*/
	adjList adj = {NULL, 0};
	initAdjList(&adj, numNodes, sizeof(int));
	int i,j,k,max,sumDegrees,degree[numNodes];
	int len = 0;

	/*construct adjacency list for the edge list*/
	constructAdjList(&adj,edgeList);

	while(1){
		sumDegrees = 0;

		for(i=0; i<numNodes; i++) {
			degree[i] = (adj.l[i]).length;
			sumDegrees += degree[i];
		}

		if(sumDegrees == 0) break;

		max = degree[0];
		k = 0;

		/*find the vertex with highest degree*/
		for(j=1; j<numNodes; j++){
			if(max < degree[j]){
				max = degree[j];
				k = j;
			}
		}

		param->vc[len] = k;
		len++;
		removeVertexK(&adj,k);
	}

	qsort(param->vc, len, sizeof(int), compare);
	param->vcSize = len;
	freeAdjList(&adj);
	return NULL;
}