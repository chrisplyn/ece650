/*
 * list.c
 *
 *      Author: Yinuo Liu
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
   } else {
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
