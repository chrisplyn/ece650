#include "approx_2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Pick an edge <u,v>, and add both u and v to your vertex cover. Throw away all edges
attached to u and v. Repeat till no edges remain. We will call this algorithm APPROX-
VC-2.*/

void* approx2(void *parameters) {
	
	/*read thread function argument*/
	thread_function_args *param = (thread_function_args *)parameters;
	int numNodes = param->numNodes;
	list *edgeList = param->edgeList;

	listNode *cur = edgeList->head;
	edge *e;

	int j;
	int len = 0;	
	
	if(param->vc == NULL) {
		param->vc = (int *)malloc(numNodes*sizeof(int));
	}

	loop:while(cur != NULL){
		e = (edge *) cur->data;

		/*check either u or v is already the vertex cover*/
		/*if yes, check next edge in the edgeList*/
		for(j=0; j<len; j++) {
			if(e->p1 == param->vc[j] || e->p2 == param->vc[j]) {
				cur = cur->next;
				goto loop;
			}
		}

		param->vc[len] = e->p1;
		len++;
		param->vc[len] = e->p2;
		len++;		
		cur = cur->next;
	}	

	qsort(param->vc, len, sizeof(int), compare);
	param->vcSize = len;
	return NULL;
}
