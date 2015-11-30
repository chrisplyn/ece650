#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include "adjacencyList.h"

#define MAX_TRIALS 25
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

static int maxNumStreet = 10;
static int maxNumLineSeg = 5;
static int maxWaitTime = 5;
static int maxCoordRange = 20;


void parseArg(int argc,char *argv[]){
	int i;
	for(i = 1; i < argc; i+=2){
		if(strcmp(argv[i],"-s") == 0){
			maxNumStreet = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-n") == 0){
			maxNumLineSeg = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-l") == 0){
			maxWaitTime = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-c") == 0){			
			maxCoordRange = atoi(argv[i+1]);
		}
	}
}


int generateRandHelper(int max) {
  unsigned int
    num_bins = (unsigned int) max + 1,
    num_rand = (unsigned int) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

    int fd = open("/dev/urandom", O_RDONLY);	
	int x;
  	
  	do {
   		read(fd, &x, sizeof(unsigned int));
  	}while (num_rand - defect <= (unsigned int)x);

  	close(fd);
  	return x/bin_size;
}


int generateRand(int min, int max){
	return min + generateRandHelper(max - min + 1);
}
 
int orientation(endpoint* p, endpoint* q, endpoint* r){
    int val = (q->y - p->y) * (r->x - q->x) -
              (q->x - p->x) * (r->y - q->y);
 
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}
 
boolean compareEndpoint(endpoint *e1, endpoint *e2){
	if(e1->x == e2->x && e1->y == e2->y) {
		return TRUE;
	}
	return FALSE;
} 

boolean isZeorLength(lineSeg *ls){
	return (compareEndpoint(&ls->e1,&ls->e2));
}

boolean onSegment(endpoint* p, endpoint* q, endpoint* r) {
    if (q->x < max(p->x, r->x) && q->x > min(p->x, r->x) &&
        q->y < max(p->y, r->y) && q->y > min(p->y, r->y))
       return TRUE;

    return FALSE;
}

boolean isColinear(lineSeg *ls1, lineSeg *ls2){
	endpoint *p1 = &ls1->e1;
	endpoint *q1 = &ls1->e2;
	endpoint *p2 = &ls2->e1;
	endpoint *q2 = &ls2->e2;

	if(compareEndpoint(p1,p2) && compareEndpoint(q1,q2)) return TRUE;

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) /*two line segments intersect*/
        return FALSE;

    /*two line segments overlap*/
    if (o1 == 0 && onSegment(p1, p2, q1)) return TRUE;
    if (o2 == 0 && onSegment(p1, q2, q1)) return TRUE;
    if (o3 == 0 && onSegment(p2, p1, q2)) return TRUE;
    if (o4 == 0 && onSegment(p2, q1, q2)) return TRUE;

    /*neither overlap nor intersect*/
    return FALSE;
}


boolean accecptLineSeg(lineSeg *ls, list *l){
	listNode* cur = l->head;
	lineSeg* ls1 = NULL;

	while(cur != NULL){
		ls1 = (lineSeg *)cur->data;

		if(isColinear(ls,ls1)) {
			return FALSE;
		}

		cur = cur->next;
	}
	return TRUE;
}

adjList adj = {NULL, 0};
int main(int argc, char *argv[]){
	parseArg(argc,argv);
	endpoint tmp;
	lineSeg ls;
	int numFailures;

	for(; ;){
		int numStreet = generateRand(2,maxNumStreet);
		int waitTime = generateRand(2,maxWaitTime);
		int street[numStreet];

		if(adj.l != NULL) {
			freeAdjList(&adj);
		}

		initAdjList(&adj, numStreet, sizeof(lineSeg));
		int j,k,m;
		numFailures = 0;
		
		for(j = 0; j < numStreet; j++){ /* for each street */
			street[j] = generateRand(2,maxNumLineSeg);

			for(k = 0; k < street[j]; k++){

				/*check the validity of this line segment*/
				while(1) {
					if (numFailures >= MAX_TRIALS) {
						fprintf(stderr, "Exceed maximum number trials to generate one street spec\n");
						freeAdjList(&adj);
						exit(1);
					}

					if(k == 0) {
						ls.e1.x = generateRand(-maxCoordRange,maxCoordRange);
						ls.e1.y = generateRand(-maxCoordRange,maxCoordRange);
						ls.e2.x = generateRand(-maxCoordRange,maxCoordRange);
						ls.e2.y = generateRand(-maxCoordRange,maxCoordRange);
					}
					else {
						ls.e1.x = tmp.x;
						ls.e1.y = tmp.y;
						ls.e2.x = generateRand(-maxCoordRange,maxCoordRange);
						ls.e2.y = generateRand(-maxCoordRange,maxCoordRange);
					}

					if(!isZeorLength(&ls)) { /*length not zero*/
						boolean accepted = TRUE;

						for(m=0; m<=j; m++) { /*go through current adjacency list*/
							if(!accecptLineSeg(&ls,adj.l+m)){
								accepted = FALSE;
								numFailures++;
								break;
							}
						}
						if(accepted) { /*ls is accepted, append to street j's list*/
							append(adj.l+j,&ls);
							tmp.x = ls.e2.x;
							tmp.y = ls.e2.y;
							break;
						}
					}
					else {
						numFailures++;
					}
				}
			}
		}
		printAdjList(&adj);
		freeAdjList(&adj);
		printf("g\n");
		printf("r\n");
		fflush(stdout);
		sleep(waitTime);
	}
}



