#include <stdio.h>

/* This is the function from page 37 in the learning Guide */
int bitCount(unsigned long x){ 
    int count = 0;
    if(x == 0)
        return 0;

    for(; x != 0; x >>= 1){
        if (x & 01) 
            count++;
    }
    printf("The number of corresponding bits is: %d\n", count);
    return count;
}

int main(){
    unsigned long x;
    unsigned long y;
    printf("Hello, this is a friendly message\nPlease enter the first number\n");
    scanf("%lu", &x);
    printf("The first number is %lu\n", x);

    printf("Please enter the second number\n");
    scanf("%lu",&y);
    printf("The second number is %lu\n", y);

    bitCount(x & y);
    return 0;
}