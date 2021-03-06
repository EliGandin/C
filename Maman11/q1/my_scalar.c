#include <stdio.h>
#include <stdlib.h>

/* This function runs in 𝚯(n) */
int scalar_product(int a[], int b[], int n)
{
    int i;
    int sum = 0;
    for (i = 0; i < n; ++i)
    {
        sum += a[i] * b[i];
    }
    printf("Scalar product is %d\n", sum);
    return sum;
}

int main()
{
    int n;
    int i;
    int *a;     
    int *b;

    printf("Hello, this is a friendly message.\nPlease enter the dimension of the vectors\n");
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("Invalid Dimenstion\n");
        return 0;
    }

    a = (int *)malloc(n * sizeof(int));     /* We allocate memory according to the dimension of the vector*/
    if (a == NULL)          
    {
        printf("Unable to malloc \'a\' array\n");       /* Make sure the memory is allocted */
        return 0;
    }
    b = (int *)malloc(n * sizeof(int));     /* We allocate memory according to the dimension of the vector*/
    if (b == NULL)
    {
        printf("Unable to malloc \'b\' array\n");
        free(a);        /* Free previously allocated memory */
        return 0;
    }

    printf("Please enter the first vector, one number at a time\n");
    for (i = 0; i < n; ++i)
    {
        scanf("%d", &a[i]);
    }

    printf("Please enter the second vector, one number at a time\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &b[i]);
    }

    scalar_product(a, b, n);

    if (a != NULL)
    {
        free(a);        /* Free memory if allocated */
    }
    if (b != NULL)
    {
        free(b);        /* Free memory if allocated */
    }
    return 0;
}