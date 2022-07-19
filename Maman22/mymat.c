#include "mymat.h"

char *MAT_Commands[E_LAST_COMMAND] = {"read_mat", "print_mat", "add_mat", "sub_mat", "mul_mat", "mul_scalar", "trans_mat", "stop"};
char *MAT_List[E_LAST_MAT] = {"MAT_A", "MAT_B", "MAT_C", "MAT_D", "MAT_E", "MAT_F"};
char *MAT_Errors[E_Last_ERROR] = {"Unidefined Matrix Name", "Unidefined Command Name", "Argument Isn't a Real Number", "Extraneous Text After End of Command", "Missing Argument", "Illegal Comma", "Argument Isn't a Scalar", "No Error"};

void add_mat(MAT *a, MAT *b, MAT *c)
{
    int i;
    int j;
    MAT h;
    
    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            h.mat[i][j] = (a->mat[i][j]) + (b->mat[i][j]);
        }
    }
    c = &h;
    print_mat(c);
}

void sub_mat(MAT *a, MAT *b, MAT *c)
{
    int i;
    int j;
    MAT h;

    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            h.mat[i][j] = (a->mat[i][j]) - (b->mat[i][j]);
        }
    }
    c = &h;
    print_mat(c);
}

void mul_mat(MAT *a, MAT *b, MAT *c)
{
    int i;
    int j;
    MAT h;

    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            h.mat[i][j] = (a->mat[i][j]) * (b->mat[j][i]);
        }
    }
    c = &h;
    print_mat(c);
}

void mul_scalar(MAT *a, float b, MAT *c)
{
    int i;
    int j;
    MAT h;

    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            h.mat[i][j] = a->mat[i][j] * b;
        }
    }
    c = &h;
    print_mat(c);
}

void trans_mat(MAT *a, MAT *b)
{
    int i;
    int j;
    MAT h;

    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            h.mat[i][j] = a->mat[j][i];
        }
    }
    b = &h;
    print_mat(b);
}

void clear_mat(MAT *a)
{
    int i;
    int j;
    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            a->mat[i][j] = 0;
        }
    }
}