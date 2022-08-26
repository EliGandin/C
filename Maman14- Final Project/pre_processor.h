#include "header.h"

/*macro arr will hold the macro lines, connected by linked list*/
typedef struct node * pt;
typedef struct node {
    String mLine;
    pt next;
} macroArr;

/*the macro table object, that will */
typedef struct tables {
    String name;
    macroArr content;
} Table;

/*linked list functions */
void add2StrList(pt *nextPtr, String line);

void freeList(pt *hptr);

/*macro process function*/
int isOldMacro(char *word, Table macTable[], int size, FILE *newFile);
