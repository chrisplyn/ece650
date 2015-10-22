/*
 * main.c
 *
 *  Created on: 2015Äê10ÔÂ20ÈÕ
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "queue.h"
#include "adjacencyList.h"


void bfs(adjList* adj, int numNodes, int origin, int* prev) {
	boolean* visited = (boolean *) malloc(numNodes*sizeof(boolean));
	int i;
	for (i = 0; i < numNodes; ++i) {
		visited[i] = FALSE;
		prev[i] = -1;
	}

	visited[origin] = TRUE;
	queue q;
	createQueue(&q, sizeof(int), NULL);
	enqueue(&q, &origin);

	while (!emptyQueue(&q)) {
		void *data = dequeue(&q);
		int u = *(int *) data;
		listNode* current = (adj->l[u]).head;

		while (current != NULL) {
			int v = *(int *)current->data;

			if (visited[v] == FALSE) {
				visited[v] = TRUE;
				enqueue(&q, &v);
				prev[v] = u;
			}

			current = current->next;
		}
		free(data);
	}
	freeQueue(&q);
   free(visited);
}

int main(int argc, char *argv[]) {
   char *line = NULL, *p;
   size_t size;
   int numNodes, e1, e2, start, dest,*prev;
   adjList adj;
   int newline;

   while ( (newline = getline(&line, &size, stdin)) != EOF) {
      switch(*line) {
         case 'V':
            sscanf(line+2, "%d", &numNodes);
            initAdjList(&adj, numNodes, sizeof(int));
            break;
         case 'E':
            p = line+2;

            while(*p != '}') {
               sscanf(p+1,"<%d,%d>",&e1,&e2);
               append(adj.l+e1,&e2);
               append(adj.l+e2,&e1);
               while(*p++ != '>');	// move pointer to position right after '>'
            }

            break;
            case 's':
               sscanf(line+2,"%d %d",&start,&dest);
               list result;
               createList(&result, sizeof(int), NULL);

               if(start >= numNodes || start < 0) {
                  printf("Error: Source vertex %d does not exist!\n", start);
                  goto CLEANUP;
               }

               if(dest >= numNodes || dest < 0) {
                  printf("Error: Destination vertex %d does not exist!\n", dest);
                  goto CLEANUP;
               }

               prev = (int *)malloc(numNodes*sizeof(int));
               bfs(&adj, numNodes, start, prev);
               int cur = dest;

               while(prev[cur] != -1) {
                  prepend(&result,&cur);
                  cur = prev[cur];
               }

               if(cur == start) {
                  prepend(&result,&cur);
               }
               else {
                  printf("Error: Path between vertex %d and vertex %d does not exist!\n", start, dest);
                  goto CLEANUP;
               }

               listNode *node = result.head;

               while(node != result.tail) {
                  printf("%d-", *(int *) node->data);
                  node = node->next;
               }
               printf("%d\n", *(int *) result.tail->data);

               CLEANUP:
                  if(result.length != 0) {
                     destroy(&result);
                  }

                  freeAdjList(&adj);
                  free(prev);
                  break;
      }
   }

   return 0;
}
