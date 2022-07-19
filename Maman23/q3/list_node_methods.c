#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lists_c.h"


void createNodeList(Node *tmp, char buff[], int len)
{
    strncpy(tmp->buffer, buff, len);
}

void PrintNode(Node *node)
{
    printf("%s", node->buffer); 

    printf("\n");
}

void PrintList(LinkedList *list)
{
    Node *curr = list->head;
    do
    {
        PrintNode(curr);
        curr = curr->next;
    } while (curr != NULL);
    printf("\n");
}

void FreeList(LinkedList *list){
    Node *curr = list->head;
    Node *tmp;

   while (curr != NULL)
    {
       tmp = curr;
       curr = curr->next;
       free(tmp);
    }
}