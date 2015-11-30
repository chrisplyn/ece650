/*
 * list.h
 *
 *      Author: Yinuo Liu
 */

#ifndef LIST_H_
#define LIST_H_

typedef void (*freeFunction)(void *);

typedef enum { FALSE=0, TRUE } boolean;

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

void createList(list *, int elementSize, freeFunction freeFn);
void destroy(list *l);
void* head(list *l,boolean removeFromList);
void append(list *, void *element);
void prepend(list *list, void *element);
int size(list *l);
boolean isEmptyList(list *);

#endif /* LIST_H_ */
