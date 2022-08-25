/**
 * assembler.c contains main function of the assembler.
 * the program works in 3 steps, and translates assembly sentences to binary by set of rules.
 * most of the modified objects are built here, and they get sent to each pass by need.
 * the 3 passes are pre assembler, first pass and second pass.
 * author: Or Badani 316307586, Eli Gandin 209327915
 */

#include "header.h"

int main(int argc, char *argv[]) {
    FILE *sourceFile;
    String sourceName,fileName;
    symbol *symTable;
    object *objects;

    while ((--argc >= 1)) {

        objects = (object *) malloc(sizeof(object));
        objects->memory= (sysReserved *) malloc(sys_res_size * sizeof(sysReserved));
        objects->actionTable = (action *) malloc(instructions_size * sizeof(action));
        objects->memVariables= (dynamicMem *) malloc(memVariables_size * sizeof(dynamicMem));
        memoryBuild(objects->memory);
        tableBuild(objects->actionTable);

        strcpy(sourceName, *++argv);
        strcpy(fileName, sourceName);

        if ((sourceFile = fopen(strcat(fileName, ".as"), "r")) == NULL) {
            printf("Argument is invalid\n");
            printf("program terminated by an error.\n");
        }
        else {

            fileName[strlen(fileName) NEUTRAL] = 'm'; /*the filename will be ready with .am*/
            pre_processor(sourceFile, fileName);

            /*sending the objects and the source name*/
            symTable = first_pass(sourceName,  objects);

            if (objects->memVariables[err_flag].num)
                printf("program terminated by an error.\n");

            else
                second_pass(sourceName,  symTable, objects);

            free(symTable);
            free (objects->memory);
            free (objects->actionTable);
            free (objects->memVariables);
            free(objects);

        }
    }
    return 0;
}