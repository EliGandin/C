/**
 * first_pass function builds the symbol table, to be used in second pass and makes error checking.
 * most of the error checks are being made here, and printed to screen.
 * if an error found, the program continues but will not proceed to output files building (2nd pass)
 * the idea is checking for each line every possible valid option, and store it to the sym table
 * author: Or Badani 316307586, Eli Gandin 209327915
 */

#include "first_pass.h"


symbol* first_pass(char *fileName,object* objects) {

    int IC = OPS_FIRST_VALUE, DC = DEF_VAL, tableSize = DEF_VAL, lineCounter = DEF_VAL, regCounter = DEF_VAL, symbolInd = NEUTRAL, actionInd = NEUTRAL, memInd = NEUTRAL, refNum=DEF_VAL;
    int errorFlag = FALSE, programFailure=FALSE, newSymbol = FALSE;

    String line, copyLine, validLabel;
    char delimit[] = " \n\t\r ,";
    char *token,*r, *operandCopy, *structLabel;

    symbol *q;
    symbol *symTable = (symbol *) malloc(sizeof(symbol));
    operandCopy=(char*)malloc(sizeof (char *));
    structLabel=(char *) malloc (sizeof (char *));

    FILE *sFile = fopen(strcat(fileName,".am"), "r");
    fileName[strlen(fileName)-3]='\0'; /*removing the .am extension */

    if (!symTable) {
        printf("Memory allocation for symbol table failed\n");
        exit(0);
    }

    while (fgets(line, LINE_LENGTH, sFile) != NULL) {
        lineCounter++;
        /* Checking the line length */
        if (line[strlen(line) - 1] != '\n') {
            printf("Error in line %d: line is longer than valid line length\n",lineCounter);
            errorFlag = TRUE;
        }

        /*TRUE means we found a label during the previous iteration, so we need to expand the table by one*/
        if (newSymbol) {
            tableSize++;
            q = (symbol *) realloc(symTable, sizeof(symbol) * (tableSize + 1));
            if (!q) {
                printf("Memory allocation for symbol table failed\n");
                exit(0);
            }
            symTable = q;
        }
        newSymbol = FALSE;

        /*skipping comments*/

        strcpy(copyLine, line);
        token = strtok(copyLine, "\n"); /* In the beginning token gets the full line, and checks if it is empty */
        regCounter=0;
        /* Counting and then skipping empty\whitespaces\comment lines */
        if (!token)
            continue;

        while (isspace((unsigned char) *token)) token++;
        if (strcmp("", token) == 0||line[0]==';')
            continue;

        token = strtok(copyLine, delimit); /*token gets the first word */



        /* If found : in the line, there is a label*/
        if (strchr(line, ':')) {  /*Label check:*/
            if (!strchr(token, ':')){
                printf("Error in line %d:there is a space before the end of label declaration.\n", lineCounter);
                errorFlag = TRUE;
                token = strtok(NULL, delimit);
            }
            strcpy(validLabel, token);
            validLabel[strlen(validLabel) - 1] = '\0'; /* In order to manipulate the ":" */

            /* Error flag will be different from 0 if there was a flag beforehand or the given symbol isn't a valid symbol */
            errorFlag = errorFlag + isValidSymbol(validLabel, symTable, tableSize, objects->memory, lineCounter);
            /* As for now we'll enter the address and name to the symbol table */
            addSymbol(validLabel, IC + DC, NEUTRAL, tableSize, symTable);
            token = strtok(NULL, delimit); /*getting the word after the label*/
            newSymbol = TRUE;
        }

        actionInd = findAction(token, objects->actionTable); /* Storing the action index if there is*/


        /*dealing with data*/
        if (token[0] == '.') {
            if (!strcmp(token, ".data")) {
                token = strtok(NULL, " ,\n"); /* Operand */
                while (token) { /* DC counts all the lines will be needed for the data */
                    token = strtok(NULL, delimit);
                    DC++;
                }
            }
            else if (!strcmp(token, ".string")) {

                token = strtok(NULL, delimit);

                if (token[0] == '\"' && token[strlen(token) - 1] == '\"') /* Counting the string's length */
                    DC = DC + (int) strlen(token) - 1;

                else {
                    printf("error in line %d:the string %s is invalid.\n", lineCounter, token);
                    errorFlag = TRUE;
                }
                token = strtok(NULL, delimit); /*checking if the line is empty after the string name*/
                if (token != NULL) {
                    printf("error in line %d: more than one parameter after .string\n",lineCounter);
                    errorFlag = TRUE;
                }
            }

                /* If .struct is presented */
            else if (!strcmp(token, ".struct")) {
                token = strtok(NULL, delimit);
                if (token[0] == '\"'){
                    printf("error in line %d: a struct cannot start with a string.\n",lineCounter);
                    errorFlag = TRUE;
                }
                if (isdigit(token[0])) { /* We check whether the 1st character is a number in .struct */
                    DC++;
                    token = strtok(NULL, ", ");
                }
                if (token[0] == '\"' && token[strlen(token) - 1] == '\"') /* Counting the string's length */
                    DC = DC + (int) strlen(token) - 1;
                else {
                    printf("error in line %d:the string %s is invalid.\n", lineCounter, token);
                    errorFlag = TRUE;
                }
                token = strtok(NULL, delimit); /*checking if the line is empty after the string name*/
                if (token != NULL) {
                    printf("error in line %d: more than one parameter after .string\n",lineCounter);
                    errorFlag = TRUE;
                }
            }

                /* Entry or Extern */

            else if (!strcmp(token, ".entry")) {
                token = strtok(NULL, delimit);
                if (token == NULL) {/* Word after .entry*/
                    printf("error in line %d: no label found after .entry\n",lineCounter);
                    errorFlag = TRUE;
                }
                else if (strchr(line,':'))                    {
                    printf("error in line %d: %s \":\" after .entry is not allowed. \n",lineCounter,token);
                    errorFlag = TRUE;
                }
                else {
                    symbolInd = isSymbol(token, symTable, tableSize);
                    if (symbolInd >= 0)
                        symTable[symbolInd].source = NEUTRAL; /*only entering neutral value if found, second pass will deal*/
                }
                token = strtok(NULL, delimit); /*checking if the line is empty after the entry name*/
                if (token != NULL) {
                    printf("error in line %d: more than one parameter after .entry\n",lineCounter);
                    errorFlag = TRUE;
                }

            }
            else if (!strcmp(token, ".extern")) {
                token = strtok(NULL, delimit); /*after extern there should be a symbol*/
                if (token == NULL) {/*word after .extern*/
                    printf("error in line %d: no label found after .extern\n",lineCounter);
                    errorFlag = TRUE;
                }
                else {/*error flag will be different from 0 if either there was a flag before or isn't a valid symbol*/
                    errorFlag = errorFlag + isValidSymbol(token, symTable, tableSize, objects->memory, lineCounter);

                    /*extern values for extern, including the label appearing after*/
                    addSymbol(token, 0,  E, tableSize, symTable);
                    newSymbol = TRUE;
                }
                token = strtok(NULL, delimit); /*checking if the line is empty after the extern name*/
                if (token != NULL) {
                    printf("error in line %d: more than one parameter after .extern \n",lineCounter);
                    errorFlag = TRUE;
                }
            }
        }

            /*if arrived here, this is a code line or error*/
        else if (actionInd==NEUTRAL) {
            printf("error in line %d: %s not a proper action name\n",lineCounter,token);
            errorFlag = TRUE;
        }
        else if (actionInd >= 0) {
            /* Updating the symbol table for action operands values */
            if (newSymbol) {
                symTable[tableSize].source = NEUTRAL;
            }
            IC++; /* First line in code is opcode*/
            if (actionInd < 14) /* Not rts or hlt */
            {
                token = strtok(NULL, delimit);
                while (token) { /* Getting the operands */
                    memInd = memCheck(token, objects->memory);
                    if (strchr(token, '.')) {
                        r = (strchr(token, '.')) +1; /* Extracting the struct's reference number as a String */
                        refNum = extractNumber(r); /* Extracting the struct's reference number (int) , to check whether there is a valid reference number (only 1 or 2) */
                        if (strchr(token, '.') && (refNum == 2 || refNum == 1)) {/* Checking the specific requirements */
                            strcpy(operandCopy,token);
                            char *pos = strchr( operandCopy, '.' );
                            int npos = (pos == NULL) ? -1 : (pos - operandCopy);
                            if (operandCopy[DEF_VAL] != '.')
                                strncat(structLabel,operandCopy,npos);
                        }
                        else {
                            printf("error in line %d: found \".\" , but %s is not eligible index addressing command. \n",
                                   lineCounter, token);
                            errorFlag = TRUE;
                        }
                    }

                    if (token[0] == '#')
                        IC++;
                    /*memInd <= 15 && memInd != NEUTRAL*/
                    else if (token[0]== 'r') {
                        regCounter++;
                        IC++;
                        if(regCounter==2) {
                            regCounter=0;
                            IC--;
                        }
                    }
                    else if(refNum){
                        if(isSymbol(structLabel,symTable,tableSize) && refNum>DEF_VAL)
                            IC=IC+2;
                    }
                    else
                        IC++;
                    token = strtok(NULL, delimit);
                    refNum=DEF_VAL;
                }
            }

        }

        if (errorFlag)
            programFailure=TRUE;
    }


/* Storing the dynamic memory variables */

free(structLabel);
free(operandCopy);
objects->memVariables[symTable_size].num=tableSize;
objects->memVariables[DC_size].num=DC;
objects->memVariables[IC_size].num=IC;
objects->memVariables[err_flag].num=programFailure;

/* Only the file need to be closed here */
fclose(sFile);

return symTable;
}


/*this function checks all the requirements from a symbol, collects the error flag and prints errors */
int isValidSymbol(String label, symbol * symTable, int tableSize, sysReserved *memory, int lineCounter) {
    int currIndex, memInd;
    int errorFlag = FALSE;
    int i;

    /*checking all the specific label requirements. if one doesnt work - a message printed and error flag turned on */
    if (label[strlen(label)-1] ==' ') {
        printf("error in line %d:the label %s cannot contain space before \":\".\n", lineCounter, label);
        errorFlag = TRUE;
    }

    if (strlen(label) > LABEL_LENGTH) {
        printf("error in line %d:the label %s is longer than allowed.\n", lineCounter, label);
        errorFlag = TRUE;
    }

    if (!isalpha(*label)) {
        printf("error in line %d:the label %s does not start with a letter.\n", lineCounter, label);
        errorFlag = TRUE;
    }

    for (i=1;i<strlen(label);i++)
        if (!isalnum(label[i])) {
            printf("error in line %d:the label %s should not contain non alphanumeric characters - \"%c\".\n", lineCounter, label,label[i]);
            errorFlag = TRUE;
        }

    if (tableSize > DEF_VAL) {      /* checking if the first word is a known label*/
        currIndex = isSymbol(label, symTable, tableSize);
        if (currIndex >= 0) {
            printf("error in line %d:the label %s already exists.\n", lineCounter, label);
            errorFlag = TRUE;
        }
    }
    /*checking if this is a system reserved word */
    memInd = memCheck(label, memory);
    if (memInd != NEUTRAL){
        errorFlag = TRUE;
        if (memInd < mem_reg_ind) /*this is a data*/
            printf("error in line %d:the label %s is a register name.\n", lineCounter, label);
        else if (memInd < mem_cmd_ind) /*this is a data*/
            printf("error in line %d:the label %s is a system command name.\n", lineCounter, label);
        else if (memInd < mem_data_ind) /*this is a data*/
            printf("error in line %d:the label %s is a system data define name.\n", lineCounter, label);
        else
            printf("error in line %d:the label %s is a system reserved name.\n", lineCounter, label);
    }

    return errorFlag;
}

/*adding a new symbol to symTable, with all the collected values available.
 * a -1 will be entered for non-collected values */
void addSymbol(String label, int address,  int source, int tableSize, symbol *symTable) {
    strcpy(symTable[tableSize].name, label);
    symTable[tableSize].address = address;
    symTable[tableSize].source = source;
}