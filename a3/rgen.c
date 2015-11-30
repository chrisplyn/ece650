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

#define MAX_STREET 50;
#define MAX_LINESEGMENT 50;

static int maxNumStreet = 10;
static int maxNumLineSeg = 5;
static int maxWaitTime = 5;
static int maxCoordRange = 20;


typedef struct {
	int x,y;
}endpoint;

typedef struct {
	endpoint e1, e2;
}lineSeg;


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

int generateRand(int min, int max){
	unsigned int random;
	int fd = open("/dev/urandom", O_RDONLY);	
	read(fd, &random, sizeof(unsigned int));
	int res = random % (max- min + 1) + min;
	close(fd);
	return res;
}


int main(int argc, char *argv[]){
	parseArg(argc,argv);
	
	while(1){
		int numStreet = generateRand(2, maxNumStreet);
		int waitTime = generateRand(5, maxWaitTime);
		int lineSeg[numStreet];
		char streetName[numStreet][20];
		//adjList adj = {NULL, 0};	/*store all streets in a adjacency list*/
		//initAdjList(&adj, numStreet, sizeof(lineSeg)); 
		
		int j,k,x,y;
		for(j = 0; j < numStreet; j++){
			sprintf(streetName[j], "%s %d", "street", j);
		}
		
		for(j = 0; j < numStreet; j++){ /* for each street */
			printf("a \"%s\" ", streetName[j]);
			lineSeg[j] = generateRand(1, maxNumLineSeg);

			for(k = 0; k <= lineSeg[j]; k++){
				x = generateRand(-maxCoordRange,maxCoordRange);
				y = generateRand(-maxCoordRange,maxCoordRange);
				printf("(%d,%d) ", x, y);
			}
			fflush(stdout);	
			printf("\n");
		}
		printf("g\n");
		printf("r\n");
		fflush(stdout);
		sleep(waitTime);	
	}

}

