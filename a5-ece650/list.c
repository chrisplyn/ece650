/*
 * list.c
 *
 *      Author: Yinuo Liu
 */

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"


void createList(list *list, int elementSize, freeFunction freeFn) {
  assert(elementSize > 0);
  list->length = 0;
  list->elementSize = elementSize;
  list->head = list->tail = NULL;
  list->freeFn = freeFn;
}

void destroy(list *list) {
   listNode *current;

   while(list->head != NULL) {
      current = list->head;
      list->head = current->next;

      if(list->freeFn) {
         list->freeFn(current->data);
      }

      free(current->data);
      free(current);
   }
   list->head = list->tail = NULL;
   list->length = 0;
}

void append(list *list, void *element){
   listNode *node = (listNode *)malloc(sizeof(listNode));
   node->data = malloc(list->elementSize);
   node->next = NULL;
   memcpy(node->data, element, list->elementSize);

   if(list->length == 0) {
     list->head = list->tail = node;
   } 
   else {
     list->tail->next = node;
     list->tail = node;
   }

   list->length++;
}

void prepend(list *list, void *element) {
   listNode *node = (listNode *)malloc(sizeof(listNode));
   node->data = malloc(list->elementSize);
   memcpy(node->data, element, list->elementSize);
   node->next = list->head;
   list->head = node;

   if(!list->tail) {
      list->tail = list->head;
   }

   list->length++;
}


void* head(list *list, boolean removeFromList) {
   assert(list->head != NULL);
   listNode *node = list->head;
   void* p = node->data;
   if(removeFromList) {
      list->head = node->next;

      if(list->head == NULL) {
         list->tail = NULL;
      }
      list->length--;
      free(node);
   }
   return p;
}

int size(list *list){
   return (list->length);
}

boolean isEmptyList(list *list) {
   if(list->length>0) {
      return FALSE;
   }
   return TRUE;
}

int compare(const void*a, const void*b){
   return  *(int*)a-*(int*)b;
}



void remove(list *list, int value) {
    listNode *currP, *prevP;
   /* For 1st node, indicate there is no previous. */
    prevP = NULL;

  /*
   * Visit each node, maintaining a pointer to
   * the previous node we just visited.
   */
    for (currP = list->head; currP != NULL; prevP = currP, currP = currP->next) {
      int tmp = *(int *) currP->data;
      
      if (tmp == value) {  /* Found it. */
        if (prevP == NULL) {
        /* Fix beginning pointer. */
          list->head= currP->next;
        } 
        else {
        /*
         * Fix previous node's next to
         * skip over the removed node.
         */
        prevP->next = currP->next;
      }

      /* decrement list size */
      list->length--;

      /* Deallocate the node. */
      free(currP->data);
      free(currP);
      return;
    }
  }
}


void printVC(int len, int* arr, const char* name){
  int i;

  printf("%s: ", name);
  for(i = 0; i < len-1; i++){
    printf("%d,", arr[i]);
    fflush(stdout);
  }

  printf("%d \n", arr[len-1]);
  fflush(stdout);
}

double timediff(struct timespec *start, struct timespec *end) {
  return (end->tv_sec-start->tv_sec)+(end->tv_nsec-start->tv_nsec)/BILLION;
}
