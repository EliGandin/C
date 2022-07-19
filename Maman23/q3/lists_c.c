#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists_c.h"

int HandleBuffer(LinkedList *list, char buff[], char ch, int *index)
{
    if (*index == (BUFFER_SIZE)) /* Creates a node and resets the buffer */
    {
        Node *tmp = (Node *)malloc(sizeof(Node));
        createNodeList(tmp, buff, *index);

        if (list->tail == NULL) /* If the list is empty */
        {
            list->head = tmp;
            list->tail = list->head;
        }
        else
        {
            list->tail->next = tmp;
            list->tail = tmp;
            list->tail->next = NULL;
        }

        memset(buff, 0, BUFFER_SIZE); /* Clearing the buffer */
        *index = 0;
    }
    else
    {
        if (ch == EOF)
        {
            Node *tmp = (Node *)malloc(sizeof(Node));
            createNodeList(tmp, buff, *index);

            if (list->tail == NULL) /* If the list is empty */
            {
                list->head = tmp;
                list->tail = list->head;
                return 0;
            }
            else
            {
                list->tail->next = tmp;
                list->tail = tmp;
                list->tail->next = NULL;
                return 0;
            }
        }
        buff[*index] = ch;
        (*index)++;
    }

    return 0;
}

int main()
{
    char ch;
    int i = 0;
    char buff[BUFFER_SIZE];
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    FILE *a1;

    a1 = fopen("a1.txt", "r");
    if (a1 == NULL)
    {
        printf("Unable to open file \"a1.txt\"\n");
        return 0;
    }

    do
    {
        ch = fgetc(a1);

        HandleBuffer(list, buff, ch, &i);
        if (feof(a1))
        {
            break;
        }

    } while (1);

    fclose(a1);

    printf("The content of \"a1.txt\"\n");
    PrintList(list);
    FreeList(list);

    return 0;
}