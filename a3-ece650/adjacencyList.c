/*
 * adjacencyList.c
 * Author: Yinuo Liu
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
   char street[20];

   for (i=0; i<adj->numNodes; ++i) {
      sprintf(street, "%s %d", "street ", i);
      printf("a \"%s\" ", street);
      current = ((adj->l)[i]).head;

      while(current != NULL) {
         lineSeg *ls = (lineSeg *) current->data;

         if(current == (adj->l+i)->head) {
            printf("(%d,%d) (%d,%d) ",ls->e1.x,ls->e1.y,ls->e2.x,ls->e2.y);
         }
         else {
            printf("(%d,%d) ",ls->e2.x,ls->e2.y);
         }
         current = current->next;
      }
      printf("\n");
   }
}
