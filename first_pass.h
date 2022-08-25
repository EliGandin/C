#include "header.h"

/*to navigate through the memory list*/
enum memory_index{mem_data_ind=34 ,mem_cmd_ind=32, mem_reg_ind=8 };

/*symbol table functions */

int isValidSymbol(String label, symbol * symTable, int tableSize, sysReserved *memory, int lineCounter);

void addSymbol(String label, int address, int source, int tableSize, symbol *symTable);