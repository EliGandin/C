#include "pre_proccesor.h"

void FreeList(LinkedList *list){
    node *curr = list->head;
    node *tmp;

   while (curr != NULL)
    {
       tmp = curr;
       curr = curr->next;
       free(tmp);
    }
}