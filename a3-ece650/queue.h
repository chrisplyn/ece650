/*
 * queue.h
 *
 *  Created on: 2015Äê10ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "list.h"

typedef struct {
   list queueList;
   freeFunction freeFn;
} queue;

void createQueue(queue *q, int elementSize, freeFunction freeFn);
void enqueue(queue *q, void *element);
void* dequeue(queue *q);
void freeQueue(queue *q);
int sizeQueue(queue *q);
boolean emptyQueue(queue *q);

#endif /* QUEUE_H_ */
