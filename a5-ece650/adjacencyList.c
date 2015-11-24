/*
 * adjacencyList.c
 *
 *      Author: Yinuo Liu
 */
#include <stdlib.h>
#include <stdio.h>

#include "adjacencyList.h"

void initAdjList(adjList *adj, int numNodes, int elementSize) {
	if (numNodes == 0) {
	   return;
	}

	int i;
	adj->numNodes = numNodes;
	adj->l = (list *)malloc(numNodes*sizeof(list));

   if(!adj->l) {
      return;
   }
   
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

void constructAdjList(adjList *adj, list *edgeList) {
   listNode* cur = edgeList->head;

   while(cur != NULL) {
      edge *e = (edge *) cur->data;
      append(adj->l+(e->p1),&(e->p2));
      append(adj->l+(e->p2),&(e->p1));
      cur = cur->next;
   }
}


void removeVertexK(adjList *adj, int k) {
   listNode* cur = (adj->l+k)->head;

   while(cur != NULL) {
      int v = *(int *)cur->data;
      remove(adj->l+v,k);  /*remove k from its neighbors' adjacency list*/
      cur = cur->next;        
   }

   /*remove all edges incident on vertex k*/
   destroy(adj->l+k);
}