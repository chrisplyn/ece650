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

typedef struct {
	int x,y;
}endpoint;

typedef struct {
	endpoint e1, e2;
}lineSeg;


void initAdjList(adjList *, int, int);
void freeAdjList(adjList *);
void printAdjList(adjList *);

#endif /* ADJACENCYLIST_H_ */
