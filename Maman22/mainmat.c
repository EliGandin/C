#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "mymat.h"

#define BUFFER_SIZE 100

void show_promt_message(void)
{
    printf("\r\n\nPlease press ENTER to continue\n\n");

    /* Wait for ENTER to continue*/
    getchar();
}

void clear_status(InputParam *input_params)
{
    memset((void *)input_params, 0, sizeof(input_params));
    input_params->mats[0] = E_LAST_MAT;
    input_params->mats[1] = E_LAST_MAT;
    input_params->mats[2] = E_LAST_MAT;
    input_params->index = 0;
}

void clear_MATs(MAT *mat_a, MAT *mat_b, MAT *mat_c, MAT *mat_d, MAT *mat_e, MAT *mat_f)
{
    memset(mat_a, 0, sizeof(MAT));
    memset(mat_b, 0, sizeof(MAT));
    memset(mat_c, 0, sizeof(MAT));
    memset(mat_d, 0, sizeof(MAT));
    memset(mat_e, 0, sizeof(MAT));
    memset(mat_f, 0, sizeof(MAT));
}

char *trimString(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';

    return str;
}

int convert_command(int cmd)
{
    switch (cmd)
    {
    case 0:
        /* Read Matrix */
        return (int)E_read_mat;
    case 1:
        /* Print Matrix */
        return (int)E_print_mat;
    case 2:
        /* Add Matrices */
        return (int)E_add_mat;
    case 3:
        /* Sub Matrices */
        return (int)E_sub_mat;
    case 4:
        /*Multiply Matrices */
        return (int)E_mul_mat;
    case 5:
        /* Multiply by scalar */
        return (int)E_mul_scalar;
    case 6:
        /* Transpose */
        return (int)E_trans_mat;
    case 7:
        /* Transpose */
        return (int)E_stop;
    default:
        return 0xFFFFFFFF;
    }
}

int command_input(char *argsBuffer[], InputParam *param)
{
#define SPACE_DELIMITER 0x20
    char *ptr = NULL;
    char command_buffer[50];
    int i;

    ptr = strchr(*argsBuffer, SPACE_DELIMITER);
    if (ptr)
    {
        memcpy(command_buffer, *argsBuffer, ptr - *argsBuffer);
        command_buffer[ptr - *argsBuffer] = '\0';

        for (i = 0; i < E_LAST_COMMAND; ++i)
        {
            if (strcmp(command_buffer, MAT_Commands[i]) == 0)
            {
                param->command = (E_command)convert_command(i);
                *argsBuffer += strlen(command_buffer) + 1;
                return 0;
            }
        }
    }
    else
    {
        if (strcmp(*argsBuffer, MAT_Commands[7]) == 0)
        {
            param->command = (E_command)convert_command(7);
            return 0;
        }
    }
    return 1;
}

int get_next_param(char *argsBuffer[], InputParam *param)
{
#define COMMA_DELIMITER 0x2C
    char *ptr = *argsBuffer;
    char command_buffer[50];
    int i;
    size_t len;

    ptr = trimString(ptr);
    if ((ptr != *argsBuffer) || (strlen(*argsBuffer) != strlen(ptr)))
    {
        len = strlen(ptr);
        memcpy(*argsBuffer, ptr, len);
        *((*argsBuffer) + len) = '\0';
    }

    ptr = strchr(*argsBuffer, COMMA_DELIMITER);
    if (ptr)
    {
        if (((param->command == E_add_mat) || (param->command == E_sub_mat) ||
             (param->command == E_mul_mat) || (param->command == E_mul_scalar)) &&
            (param->index == 2))
        {

            memcpy(command_buffer, ptr + 1, strlen(*argsBuffer) - (ptr - *argsBuffer));
            command_buffer[strlen(*argsBuffer) - (ptr - *argsBuffer)] = '\0';
            strcpy(*argsBuffer, command_buffer);

            ptr = *argsBuffer;
            ptr = trimString(ptr);
            if ((ptr != *argsBuffer) || (strlen(*argsBuffer) != strlen(ptr)))
            {
                len = strlen(ptr);
                memcpy(*argsBuffer, ptr, len);
                *((*argsBuffer) + len) = '\0';
            }
            return 0;
        }

        memcpy(command_buffer, *argsBuffer, ptr - *argsBuffer);
        command_buffer[ptr - *argsBuffer] = '\0';

        for (i = 0; i < E_LAST_MAT; ++i)
        {
            if (strcmp(command_buffer, MAT_List[i]) == 0)
            {
                param->mats[param->index++] = (E_Mats)i;
                *argsBuffer += strlen(command_buffer) + 1;

                return 0;
            }
        }
    }
    else
    {
        for (i = 0; i < E_LAST_MAT; ++i)
        {
            if (strcmp(*argsBuffer, MAT_List[i]) == 0)
            {
                param->mats[param->index++] = (E_Mats)i;
                *argsBuffer += strlen(command_buffer) + 1;

                return 0;
            }
        }
    }
    return 1;
}

int check_command(char *argsBuffer[], InputParam *param)
{
    if ((param->command == E_print_mat) && (param->index == 0))
    {
        return (int)E_Unidefined_Matrix_Name;
    }
    if ((param->command == E_print_mat) && (param->index == 1) && (strlen(*argsBuffer)))
    {
        return (int)E_Extraneous_Text_After_End_of_Command;
    }
    if ((param->command == E_add_mat) && (param->index == 1))
    {
        return (int)E_Missing_Argument;
    }
    if (((param->command == E_add_mat) || (param->command == E_sub_mat)) && ((param->index == 1) || (param->index == 2)))
    {
        return (int)E_Missing_Argument;
    }
    return (int)E_No_Error;
}

void print_mat(MAT *m)
{
    int i;
    int j;
    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            printf("%f\t", m->mat[i][j]);
        }
        printf("\n");
    }
}

MAT *get_data(InputParam *param, MAT *mat_a, MAT *mat_b, MAT *mat_c, MAT *mat_d, MAT *mat_e, MAT *mat_f)
{
    if (param->index == 0)
    {
        return NULL;
    }

    switch (param->mats[param->index - 1])
    {
    case E_MAT_A:
        return mat_a;
    case E_MAT_B:
        return mat_b;
    case E_MAT_C:
        return mat_c;
    case E_MAT_D:
        return mat_d;
    case E_MAT_E:
        return mat_e;
    case E_MAT_F:
        return mat_f;
    default:
        return NULL;
    }
}

void launch_command(InputParam *param, MAT *mat_a, MAT *mat_b, MAT *mat_c, MAT *mat_d, MAT *mat_e, MAT *mat_f)
{
    MAT *ptr_mat1 = NULL;
    MAT *ptr_mat2 = NULL;
    MAT *ptr_mat3 = NULL;

    switch (param->command)
    {
    case E_print_mat:
        ptr_mat1 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        if (ptr_mat1)
        {
            print_mat(ptr_mat1);
        }
        break;
    case E_add_mat:
        param->index = 1;
        ptr_mat1 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 2;
        ptr_mat2 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 3;
        ptr_mat3 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);

        add_mat(ptr_mat1, ptr_mat2, ptr_mat3);

        clear_status(param);
        clear_MATs(mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        break;
    case E_sub_mat:
        param->index = 1;
        ptr_mat1 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 2;
        ptr_mat2 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 3;
        ptr_mat3 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);

        sub_mat(ptr_mat1, ptr_mat2, ptr_mat3);

        clear_status(param);
        clear_MATs(mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        break;
    case E_mul_mat:
        param->index = 1;
        ptr_mat1 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 2;
        ptr_mat2 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 3;
        ptr_mat3 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);

        mul_mat(ptr_mat1, ptr_mat2, ptr_mat3);

        clear_status(param);
        clear_MATs(mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        break;
    case E_mul_scalar:
        param->index = 1;
        ptr_mat1 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 2;
        ptr_mat2 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);

        mul_scalar(ptr_mat1, param->scalar, ptr_mat2);

        clear_status(param);
        clear_MATs(mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        break;
    case E_trans_mat:
        param->index = 1;
        ptr_mat1 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        param->index = 2;
        ptr_mat2 = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);

        trans_mat(ptr_mat1, ptr_mat2);

        clear_status(param);
        clear_MATs(mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
    default:
        break;
    }
}

void get_next_float_param(char *argsBuffer[], InputParam *param, MAT *ptr_mat)
{
#define COMMA_DELIMITER 0x2C
    char *ptr = *argsBuffer;
    char data_buffer[50];
    float tmp_float;
    int i;
    int j;
    size_t len;

    for (i = 0; i < LENGTH_OF_MATRIX; i++)
    {
        for (j = 0; j < LENGTH_OF_MATRIX; j++)
        {
            ptr = trimString(ptr);
            if (ptr != *argsBuffer)
            {
                len = strlen(ptr);
                memcpy(*argsBuffer, ptr, len);
                *((*argsBuffer) + len) = '\0';
            }

            ptr = strchr(*argsBuffer, COMMA_DELIMITER);
            if (ptr)
            {
                memcpy(data_buffer, *argsBuffer, ptr - *argsBuffer);
                data_buffer[ptr - *argsBuffer] = '\0';

                sscanf(data_buffer, "%f", &tmp_float);
                ptr_mat->mat[i][j] = tmp_float;
                *argsBuffer += strlen(data_buffer) + 1;
                ptr = *argsBuffer;
            }
            else
            { /* Last value */
                sscanf(*argsBuffer, "%f", &tmp_float);
                ptr_mat->mat[i][j] = tmp_float;

                print_mat(ptr_mat);

                return;
            }
        }
    }
    print_mat(ptr_mat);
}

void get_scalar(char *argsBuffer[], InputParam *param)
{
#define COMMA_DELIMITER 0x2C
    char *ptr = *argsBuffer;
    char data_buffer[50];
    float scalar;
    int len;

    ptr = trimString(ptr);
    if (ptr != *argsBuffer)
    {
        len = strlen(ptr);
        memcpy(*argsBuffer, ptr, len);
        *((*argsBuffer) + len) = '\0';
    }

    ptr = strchr(*argsBuffer, COMMA_DELIMITER);
    if (ptr)
    {
        memcpy(data_buffer, *argsBuffer, ptr - *argsBuffer);
        data_buffer[ptr - *argsBuffer] = '\0';

        sscanf(data_buffer, "%f", &scalar);
        param->scalar = scalar;
        *argsBuffer += strlen(data_buffer) + 1;
        ptr = *argsBuffer;

        sscanf(data_buffer, "%f", &scalar);
        param->scalar = scalar;
    }
}

int parse_input_parameters(char argsBuffer[], InputParam *param, MAT *mat_a, MAT *mat_b, MAT *mat_c, MAT *mat_d, MAT *mat_e, MAT *mat_f)
{
    int rc;
    MAT *ptr_mat = NULL;

    rc = command_input(&argsBuffer, param);
    if (rc != 0)
    {
        /* Wrong command */
        return (int)E_Unidefined_Command_Name;
    }

    if (param->command == E_stop)
    {
        return 0;
    }
    /*Parsing the first parameter*/
    rc = get_next_param(&argsBuffer, param);
    if (rc != 0)
    {
        /* Miss first parameter */
        rc = check_command(&argsBuffer, param);
        return rc;
    }

    if (param->command == E_print_mat)
    {
        return 0;
    }

    if (param->command == E_read_mat)
    {
        ptr_mat = get_data(param, mat_a, mat_b, mat_c, mat_d, mat_e, mat_f);
        get_next_float_param(&argsBuffer, param, ptr_mat);
        return 0;
    }

    if (param->command == E_mul_scalar)
    {
        get_scalar(&argsBuffer, param);
    }

    /* Parsing the second parameter */
    rc = get_next_param(&argsBuffer, param);
    if (rc != 0)
    {
        /* Miss second parameter */
        rc = check_command(&argsBuffer, param);
        return rc;
    }

    if ((param->command == E_trans_mat) || (param->command == E_mul_scalar))
    {
        return 0;
    }

    /*Parsing the third parameter*/
    rc = get_next_param(&argsBuffer, param);
    if (rc != 0)
    {
        /* Miss second parameter */
        rc = check_command(&argsBuffer, param);
        return rc;
    }

    if ((param->command == E_add_mat) || (param->command == E_sub_mat) ||
        (param->command == E_mul_mat) || (param->command == E_mul_scalar))
    {
        return 0;
    }
    return 0;
}

int get_input_parameters(char argsBuffer[])
{
    int inputIndex = 0;
    char ch;
    int rc = 0;
    char * ptr = argsBuffer;
    size_t len;

    while ((ch = getchar()) != EOF && (inputIndex < BUFFER_SIZE))
    {
        if (ch == '\n')
        {
            argsBuffer[inputIndex] = '\0';
            ptr = trimString(ptr);
            if (ptr != argsBuffer){
                len = strlen(ptr);
                if(len != 0){
                    break;
                }
                else{
                    inputIndex = 0;
                    continue;
                }
            }
            else{
                break;
            }
        }
        argsBuffer[inputIndex] = ch;
        inputIndex++;
    }
    return rc;
}

void print_error(int error)
{
    switch (error)
    {
    case E_Unidefined_Matrix_Name:
        printf("Unidefined Matrix Name\n\n");
        break;
    case E_Unidefined_Command_Name:
        printf("Unidefined Command Name\n\n");
        break;
    case E_Argument_Isnt_a_Real_Number:
        printf("Argument Isn't a Real Number\n\n");
        break;
    case E_Extraneous_Text_After_End_of_Command:
        printf("Extraneous Text After End of Command\n\n");
        break;
    case E_Missing_Argument:
        printf("Missing Argument\n\n");
        break;
    case E_Illegal_Comma:
        printf("Illegal Comma\n\n");
        break;
    case E_Argument_Isnt_a_Scalar:
        printf("Argument Isn't a Scalar\n\n");
        break;
    case E_No_Error:
    default:
        break;
    }

    show_promt_message();
}
void disable_canonic_keypad(struct termios *oldt, struct termios *newt)
{
    /*make_blocking(fd_keyboard);
    tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr(STDIN_FILENO, oldt);
    /*now the settings will be copied*/
    *newt = *oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt->c_lflag &= ~(ICANON);

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr(STDIN_FILENO, TCSANOW, newt);
}

void enable_canonic_keypad(struct termios *oldt)
{
    /* Change back the declaretion*/
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

int main(void)
{
    struct termios oldt;
    struct termios newt;
    MAT MAT_A;
    MAT MAT_B;
    MAT MAT_C;
    MAT MAT_D;
    MAT MAT_E;
    MAT MAT_F;

    char argsBuffer[BUFFER_SIZE];
    int command_stop = 0;
    int rc = 0;
    InputParam input_params;

    clear_status(&input_params);
    clear_MATs(&MAT_A, &MAT_B, &MAT_C, &MAT_D, &MAT_E, &MAT_F);

    disable_canonic_keypad(&oldt, &newt);

    while (command_stop == 0)
    {
        char *ptr = argsBuffer;

        /*system("clear");*/
        
        printf("Hello, this is a friendly messege\nPlease enter your input\n");

        memset(argsBuffer, 0, BUFFER_SIZE);
        rc = get_input_parameters(argsBuffer);
        if (rc != 0)
        {
            continue;
        }
        ptr = trimString(ptr);
        if ((ptr != (char *)(argsBuffer)) || (strlen((char *)(argsBuffer)) != strlen(ptr)))
        {
            strcpy(argsBuffer, ptr);
        }

        rc = parse_input_parameters(argsBuffer, &input_params, &MAT_A, &MAT_B, &MAT_C, &MAT_D, &MAT_E, &MAT_F);
        if (rc != 0)
        {
            /* Error string */
            print_error(rc);

            command_stop = 1;
        }

        if (input_params.command == (int)E_stop)
        {
            printf("Bye Bye \n\n");
            command_stop = 1;
        }

        launch_command(&input_params, &MAT_A, &MAT_B, &MAT_C, &MAT_D, &MAT_E, &MAT_F);
        if (command_stop == 0)
        {
            show_promt_message();
        }
    }
    enable_canonic_keypad(&oldt);

    return 0;
}