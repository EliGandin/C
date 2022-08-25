#include "header.h"

/*this object will hold the data lines of the ob file*/
typedef struct data_List {
    String dLine;
} dataList;

/*this object will store the extern labels addresses, for the externals file*/
typedef struct extern_addresses {
    String name;
    int address;
} extAddress;

/*enums for navigating through the different binary translating processes*/
enum operation{opcode_add=6, source_add=4, dest_add=2};

enum addressing{immediate = 0 , direct=1, structure=2, reg_direct=3 };

enum full_binary_translation{binary_line_len=10, binary_line_ind=9 };

/*for finding the indexes in the value array */
enum values_Array_index{errorFlag_ind = 0 , printCounter_ind=1 ,extAdd_ind=2, value_Array_Size=3};


/* Functions for translating to bits, handling each operand and each line's functions */

int firstWordProcess(int action, int printCounter, FILE *newCode, operandBuilder *a);

void bitTranslation(int bitLength, int index, int value, char line[]);

int stringTranslation(char * token, dataList *dataLine, int DC);

int structTranslation(char * token, dataList *dataLine, int DC); /* Ongoing Process */

int dataTranslation(char * token, dataList *dataLine, int DC);


int* middleWordProcess(char *firstOperand, char *secondOperand, operandBuilder *memBuild, symbol *symTable, int symTableSize, FILE *newCode,extAddress *extAdd,int extSize, int printCounter,int errorFlag, int regNum1, int regNum2, operandBuilder *regHandle);

int operandTranslation(char *operand, operandBuilder *operandBuilder, int isSource, int isStruct ,symbol *symTable, int symTableSize,extAddress *extAdd,int printCounter,int *extSize,int *errorFlag, int *regNum);

int labelWordsProcess(int address, int source, FILE *newCode, int printCounter);

int immediateWordProcess(char *operand, FILE *newCode, int printCounter);

/*extern and entries making files functions*/

void updateExtAdd(String label, int address, extAddress *extAdd,int extSize);

void makeExtFile(String fileName,extAddress *extAdd,int extSize);

void makeEntFile(String fileName,symbol *symTable,dynamicMem *memVariables);


