/*
 * adjacencyList.h
 *
 *      Author: Yinuo Liu
 */

#ifndef ADJACENCYLIST_H_
#define ADJACENCYLIST_H_

#include "list.h"
typedef struct {
   list* l;
   int numNodes;
} adjList;

void initAdjList(adjList *, int, int);
void constructAdjList(adjList *, list *);
void freeAdjList(adjList *);
void printAdjList(adjList *);
void removeVertexK(adjList *, int k);

#endif /* ADJACENCYLIST_H_ */
