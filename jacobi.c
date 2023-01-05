#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
void swap(uint64_t *a, uint64_t *b)
{
  uint64_t temp;
  temp = *a;
  *a = *b;
  *b = temp;
}
int8_t jacobiSymbol(uint64_t a, uint64_t n)
{
  int32_t t = 1;
  while(a != 0)
  {
    while((a%2) == 0)
    {
      a/=2;
      int32_t r = n%8;
      if((r==3) || (r==5))
        t = (-1*t);
    }
    swap(&a, &n);
    if((a%4 == 3)&&(n%4 == 3))
      t = (-1*t);
    a%=n;
  }
  if(n==1)
    return t;
  else
    return 0;
}
