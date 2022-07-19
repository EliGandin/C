#include <stdio.h>
#include <stdarg.h>

int my_and(int count,...){
    int i;
    int res;

    /* Initialization */
    va_list list;
    va_start(list, count); 
    res = va_arg(list,int);

    for (i = 1; i < count; i++){
        res = res & va_arg(list, int);
    }

    /* Memmory release */
    va_end(list);
    return res;
}

int main(void){
    int a = 12;
    int b = 2047;
    int c = 1023;
    int d = 0;
    int e = 511;
    int f = 4095;
    int g = 8191;

    int res1 = my_and(2,a,b);
    int res2 = my_and(4,a,c,b,f);
    int res3 = my_and(5,g,a,b,e,c);
    int res4 = my_and(7,a,b,c,d,e,f,g);
    /* Print res1 */
    printf("Result Num.1: %d\n", res1);
    printf("Result Num.1 in Hexadecimal: %x\n\n", res1);
    /* Print res2 */
    printf("Result Num.2: %d\n", res2);
    printf("Result Num.2 in Hexadecimal: %x\n\n", res2);
    /* Print res3 */
    printf("Result Num.3:%d\n", res3);
    printf("Result Num.3 in Hexadecimal: %x\n\n", res3);
    /* Print res4 */
    printf("Result Num.4: %d\n", res4);
    printf("Result Num.4 in Hexadecimal: %x\n\n", res4);
    
    return 0;
}
