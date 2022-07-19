#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct openu
{
  int *a1;
  char *b2;
  double c3;
  int d4;
}
Tipus;


int main()
{

  Tipus s;
  printf("%ld",sizeof(*s.a1));
}
