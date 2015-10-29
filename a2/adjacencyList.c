/*
 * adjacencyList.c
 *
 *  Created on: 2015Äê10ÔÂ21ÈÕ
 *      Author: Administrator
 */
#include <stdlib.h>
#include <stdio.h>

#include "adjacencyList.h"

void initAdjList(adjList *adj, int numNodes, int elementSize) {
	int i;
	adj->numNodes = numNodes;
	adj->l = (list *)malloc(numNodes*sizeof(list));

	for (i = 0; i < numNodes; ++i) {
		createList(adj->l+i, elementSize, NULL);
	}
}

void freeAdjList(adjList *adj) {
   int i;

   for(i=0; i<adj->numNodes; ++i) {
      destroy(adj->l+i);
   }

   free(adj->l);
   adj->l = NULL;
}

void printAdjList(adjList *adj) {
   int i;
   listNode* current;

   for (i=0; i<adj->numNodes; ++i) {
      if(isEmptyList(adj->l+i)) {
         continue;
      }

      current = ((adj->l)[i]).head;
      printf("neighbor of node:%d is ", i);
      while(current != NULL) {
         printf("%d ", *(int *) current->data);
         current = current->next;
      }
      printf("\n");
   }
}
