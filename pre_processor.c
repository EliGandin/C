/**
 * pre_assembler function searches and build macro declarations and prints it to a new .am file
 * every declaration is being stored in macro table, and implemented if the name is written
 * macTable is a dynamic object that each cell contains a macro name and a linked list of the macro lines
 * authors: Or Badani 316307586, Eli Gandin 209327915
 */


#include "pre_processor.h"

void pre_processor(FILE *sFile, char *fileName) {
    int tableSize = DEF_VAL;
    int macroFlag = FALSE, firstLine = TRUE, endMacroFlag=FALSE;

    String macroStr = "macro", endmacroStr = "endmacro",line,copyLine;
    char *token; /*will hold the broken string */
    char delimit[] = " \n\t\r";

    Table *t;
    Table *macTable = (Table *) malloc(sizeof(Table));

    FILE *newFile;
    newFile = fopen(fileName, "w");

    if (!macTable) {
        printf("memory allocation failed\n");
        exit(0);
    } else {
        macTable->content.next = NULL; /*resetting the first node pointer */
        while (fgets(line, LINE_LENGTH, sFile) != NULL) {
            strcpy(copyLine, line); /*line to be splitted ny strtok*/
            token = strtok(copyLine, "\n"); /* in the beginning token gets the full line , and checks if it is empty*/
            if (token) {
                while (isspace((unsigned char) *token)) token++;
            }


            if (!token||strcmp("", token) == 0) { /* inserting empty lines as is*/
                if (!macroFlag)
                    fputs(line, newFile);
                else {
                    if (firstLine) /*only applies for the first macro line */
                        strcpy(macTable[tableSize].content.mLine, line);
                    else /*adding the line to the array list next node */
                        add2StrList(&(macTable[tableSize].content.next), line);
                    firstLine = FALSE;
                }
                continue;
            }

            token = strtok(copyLine, delimit); /*token gets the first word */


            if (strstr(line, endmacroStr)) /* endmacro identified */
                endMacroFlag=TRUE;
            else { /*only relevant when no endmacro found*/

                if (tableSize > DEF_VAL && !macroFlag) { /* only if we have known macros*/
                    if (isOldMacro(token, macTable, tableSize, newFile) !=
                        NEUTRAL) /*not NEUTRAL means this is an old macro, so it was deployed.*/
                        continue;
                }
            }



            if (!macroFlag) { /*macro declaration was not found yet*/
                if (!strcmp(token, macroStr)) { /*now found macro declaration*/
                    token = (strtok(NULL, delimit));
                    removeTails(token);
                    macroFlag = TRUE;
                    strcpy(macTable[tableSize].name, token);

                } else
                    fputs(line, newFile); /*writing non macro content to new file*/


            } else { /* inside a macro*/
                /*found endm, so reset the variables and get ready for another macro */
                if (endMacroFlag) {
                    macroFlag = FALSE;
                    endMacroFlag = FALSE;
                    tableSize++;
                    /*expanding the table to be as the new table num */
                    t = (Table *) realloc(macTable, sizeof(Table) * (tableSize + 1));
                    if (!t) {
                        printf("memory allocation failed\n");
                        exit(0);
                    }
                    macTable = t;
                    firstLine = TRUE;
                    macTable[tableSize].content.next = NULL;
                } else { /* storing the Table content*/
                    if (firstLine) /*only applies for the first macro line */
                        strcpy(macTable[tableSize].content.mLine, line);
                    else /*adding the line to the array list next node */
                        add2StrList(&(macTable[tableSize].content.next), line);

                    firstLine = FALSE;
                }
            }
        }
        /*adding an empty line to the end of the file, to make sure last line not shorter than 80*/        fputs("\n", newFile);

        /*closing the files and freeing the memory*/

        fclose(sFile);
        fclose(newFile);
        for (; tableSize >= 0; tableSize--)
            freeList(&(macTable[tableSize].content.next));
        free(macTable);

    }
}

/*will check if the given line is a known macro, and will deploy if it is*/
int isOldMacro(char* word, Table macTable[], int size, FILE *newFile) {
    int i;
    pt curr;
    for (i = 0; i < size; i++) {
        if (!strcmp(word, macTable[i].name)) {  /* need to compare to macro names*/
            curr = &(macTable[i].content); /*easier access to the wanted array list */
            while (curr->next != NULL) {
                fputs(curr->mLine, newFile);
                curr = curr->next;
            }
            fputs(curr->mLine, newFile);
            return i;
        }
    }
    return NEUTRAL;
}

/*building the linked list for the string of each macro declaration*/
void add2StrList(pt *nextPtr, String line) {
    pt lastNode;
    pt newNode = (pt) malloc(sizeof(struct node)); /*assign memory for the new node, string num*/
    if (!newNode) {
        printf("cannot build macro list\n");
        exit(0);
    }
    strcpy(newNode->mLine, line);
    newNode->next = NULL;

    if (*nextPtr == NULL) /*this is the first line and node*/
        *nextPtr = newNode;
    else {
        lastNode = *nextPtr;
        while (lastNode->next != NULL)
            lastNode = lastNode->next;

        lastNode->next = newNode;
    }
}

/*freeing the list nodes memory*/
void freeList(pt *hptr){
    pt p;
    while (*hptr){
        p=*hptr;
        *hptr=(*hptr)->next;
        free(p);
    }
}


