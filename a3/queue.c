/*
 * queue.cc
 *
 *  Created on: 2015Äê10ÔÂ20ÈÕ
 *      Author: Administrator
 */

#include <stdlib.h>
#include "queue.h"

void createQueue(queue *q, int elementSize, freeFunction freeFn){
   //q->queueList = malloc(sizeof(list));
   createList(&(q->queueList), elementSize, freeFn);
}

void freeQueue(queue *q) {
   destroy(&(q->queueList));
}

void enqueue(queue *q, void *element) {
   append(&(q->queueList), element);
}

void* dequeue(queue *q) {
   return head(&(q->queueList), TRUE);
}

boolean emptyQueue(queue *q) {
   if(size(&(q->queueList)) > 0) {
      return FALSE;
   }
   return TRUE;
}

int sizeQueue(queue *q) {
   return (size(&(q->queueList)));
}
