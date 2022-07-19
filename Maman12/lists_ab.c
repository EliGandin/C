#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 100 /* Size of the array that stores the string */
#define ROW 8

void PrintFormat(char c[], int total, int alpha)
{
    int l = 0;
    int t;
    printf("\n");
    for (t = 0; t < total; t++)
    {
        if ((c[t] > 32) && (c[t] < 127))
        { /* Check if the character is printable*/
            printf("%c\t", c[t]);
        }
        else
        {
            switch (c[t])
            {
            case 10:
                printf("Enter\t");
                break;
            case 32:
                printf("Space\t");
                break;
            case 9:
                printf("Tab\t");
                break;
            default:
                break;
            }
        }
        l++;
        if (l == ROW)
        { /* Checks that all the lines are in the same lengths */
            l = 0;
            printf("\n");
        }
    }
    printf("\nYou have %d Alphanumeric characters in your input\n", alpha);
    printf("\nYou have %d characters in total\n", total);
}

int HandleBuffer(char c[], int *total, char ch, int *alpha, int *k)
{
    if (*total >= (*k * BUFFER_SIZE))
    { /* Allocates addinitional memory to store the string */
        (*k)++;
        c = (char *)realloc(c, (*k) * BUFFER_SIZE * sizeof(char));
        if (c == NULL)
        {
            printf("Unable to realloc\n");
            return -1;
        }
    }
    c[*total] = ch;
    if (isalnum(c[*total]))
    {
        (*alpha)++;
    }
    (*total)++;

    return 0;
}

int main()
{
    char ch;
    int alpha = 0;
    int total = 0;
    int k = 1;

    char *c;
    c = (char *)malloc(BUFFER_SIZE * sizeof(char)); /* Initalizes the memory allocation */
    if (c == NULL)
    {
        printf("Unable to allocate memory\n\r");
        return 0;
    }

    printf("Hello, this is a friendly message\nPlease type in the input:\n");

    do
    {
        ch = getchar();

        if (ch == EOF)
        {
            break;
        }
        if (HandleBuffer(c, &total, ch, &alpha, &k) == -1)
        {
            break;
        }
    } while (1);
    PrintFormat(c, total, alpha);

    if (c != NULL)
    {
        free(c);
    }
    return 0;
}