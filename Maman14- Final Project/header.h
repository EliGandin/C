#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#define LINE_LENGTH 81
#define LABEL_LENGTH 31
#define BINARY_LENGTH 11
#define FALSE 0
#define TRUE 1
#define DEF_VAL 0
#define NEUTRAL -1
#define OPS_FIRST_VALUE 100


typedef char String[LINE_LENGTH];


/*will hold the system reserved words to be checked all over the program*/
typedef struct system_reserved_words {
    char* words;
} sysReserved;

typedef struct language_reserved{
    char* letters;
} langReserved;

/*will hold the actions names and opcodes*/
typedef struct action_table {
    char* name;
    int opcode ;
} action;

/*will hold the dynamic values collected all over the program, like sizes and flags*/
typedef struct dynamic_values_and_error_flags {
    int num;
} dynamicMem;

/*a symbol table */
typedef struct Symbol {
    String name;
    int address;
    int source; /*A\R\E */

} symbol;

/*a "father" object that holds all the new objects to be sent to each pass */
typedef struct Static_Objects {
    sysReserved *memory;
    dynamicMem *memVariables;
    action *actionTable;

} object;

/* This object will store the operand-line builder ("opcode - source operand - dest operand - A\R\E" ,  for each operation */
typedef struct operandBuilder{
    int opcode;
    int source_operand;
    int destination_operand;
    int ARE;
}   operandBuilder;

/*to navigate through the objects*/

enum symbol_source{A = 0, R=2, E=1 };

enum dynamic_values_flags{symTable_size = 0, DC_size, IC_size, err_flag};

enum staticSizes{sys_res_size = 31, instructions_size=16, memVariables_size = 4};

/*token process functions */
void removeTails(char * line);

int findAction(char* token, action table[] );
int isSymbol(String token, symbol * symTable, int tableSize);
int memCheck(char* token, sysReserved memory[]);
int extractNumber (char * word);
char* specialBaseConverter(int val);

/*action table functions */
void tableBuild(action *opcode);

/*builds the memory table, with the system reserved values*/
void memoryBuild(sysReserved *memory);

/*Project's 32-base language*/
void langBuild(langReserved *base);

char* specialBaseConverter(int val);

void intTo32BasePrint(int printCounter, FILE *file ,int binaryValue);

/*int binaryConnection(operandBuilder a);*/

void binaryTo32BasePrint(int printCounter, FILE *file ,char binaryString[]);

int binaryConnection(operandBuilder a);

void memoryBuild(sysReserved *memory);

int findAction(char* token, action table[] );

/*symbol table functions */

int isSymbol(String token, symbol * symTable, int tableSize);

/*memory holder functions */

int memCheck(char* token, sysReserved *memory );

/*conversion functions */
int extractNumber (char * word);


void pre_processor(FILE *file, char *fileName);
symbol* first_pass(char *fileName,object* objects);
void second_pass (char *fileName, symbol * symTable,object * objects);
int main(int argc, char *argv[]);
#endif