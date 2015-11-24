/*
 * list.h
 *
 *      Author: Yinuo Liu
 */

#ifndef LIST_H_
#define LIST_H_

#define BILLION 1E9

typedef void (*freeFunction)(void *);

typedef enum { FALSE, TRUE } boolean;

typedef struct _listNode {
  void *data;
  struct _listNode *next;
} listNode;

typedef struct {
  int length;
  int elementSize;
  listNode *head;
  listNode *tail;
  freeFunction freeFn;
} list;


typedef struct{
	int p1,p2;
} edge;

typedef struct{
	int numNodes,vcSize,*vc;
	list* edgeList;
  double cputime;
}thread_function_args;

void createList(list *, int elementSize, freeFunction freeFn);
void destroy(list *l);
void* head(list *l,boolean removeFromList);
void append(list *, void *element);
void prepend(list *list, void *element);
int size(list *l);
boolean isEmptyList(list *);
int compare(const void*, const void*);
void remove(list *, int); 
void printVC(int, int*, const char *);
double timediff(struct timespec *, struct timespec *);

#endif /* LIST_H_ */
