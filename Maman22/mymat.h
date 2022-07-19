#ifndef __MY_MAT_H__
#define __MY_MAT_H__

typedef enum E_commands_t
{
    E_read_mat = 0,
    E_print_mat,
    E_add_mat,
    E_sub_mat,
    E_mul_mat,
    E_mul_scalar,
    E_trans_mat,
    E_stop,
    E_LAST_COMMAND
}E_command;

typedef enum mats_t
{
    E_MAT_A = 0, 
    E_MAT_B,
    E_MAT_C,
    E_MAT_D,
    E_MAT_E,
    E_MAT_F,
    E_LAST_MAT
}E_Mats;

typedef enum E_Mat_Error_t
{   
    E_No_Error = 0,
    E_Unidefined_Matrix_Name = -1,
    E_Unidefined_Command_Name = -2,
    E_Argument_Isnt_a_Real_Number = -3,
    E_Extraneous_Text_After_End_of_Command = -4,
    E_Missing_Argument = -5,
    E_Illegal_Comma = -6,
    E_Argument_Isnt_a_Scalar = -7, 
    E_Last_ERROR = 8
}E_Mat_Error;


/* Struct defenition */
typedef struct mat_type{
#define LENGTH_OF_MATRIX    4
    float mat[LENGTH_OF_MATRIX][LENGTH_OF_MATRIX];
} MAT;

/* Struct defenition */
typedef struct InputParam_type{
#define MAX_ARGS 3
    E_Mats      mats[MAX_ARGS];
    E_command   command;
    int         index;
    float       scalar;
} InputParam;

/* Prototypes */
void add_mat(MAT *a, MAT *b, MAT *c);
void sub_mat(MAT *a, MAT *b, MAT *c);
void mul_mat(MAT *a, MAT *b, MAT *c);
void mul_scalar(MAT *a, float n, MAT *b);
void clear_mat(MAT *a);
void trans_mat(MAT *a, MAT *b);
void print_mat(MAT *m);

extern char *MAT_Commands[ E_LAST_COMMAND];
extern char *MAT_List[E_LAST_MAT];
extern char *MAT_Errors[E_Last_ERROR];

#endif