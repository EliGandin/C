#ifndef __LISTS_C_H__
#define __LISTS_C_H__

typedef struct Node{
#define BUFFER_SIZE 12
    char buffer[BUFFER_SIZE];
    struct Node * next;
}Node;

typedef struct LinkedList{
    struct Node * head;
    struct Node * tail;
}LinkedList;

void createNodeList(Node *tmp, char buff[], int len);
void PrintNode(Node *node);
void PrintList(LinkedList *list);
void FreeList(LinkedList *list);

#endif