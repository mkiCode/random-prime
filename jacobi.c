#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"bn.h"
void swap(struct bn *a1, struct bn *b1)
{
  //uint64_t temp;
  struct bn tmp; bignum_init(&tmp);bignum_assign(&tmp, a1);
  //temp = *a;
  bignum_assign(a1, b1);
  //*a = *b;
  bignum_assign(b1, &tmp);
  //*b = temp;
}
 void jacobiSymbol(struct bn* a2, struct bn* n2, int8_t* jacobi1)
{
  //printf("computing jacobi symbol...\n");
  int32_t t = 1;
  struct bn a1;bignum_init(&a1);bignum_assign(&a1, a2);//bignum_from_int(&a1, a);
  struct bn n1;bignum_init(&n1);bignum_assign(&n1, n2);//bignum_from_int(&n1, n);
  struct bn r1;bignum_init(&r1);bignum_from_int(&r1, 0);
  struct bn tmp;bignum_init(&tmp);bignum_from_int(&tmp, 0);
  struct bn tmp1;bignum_init(&tmp1);bignum_from_int(&tmp1, 0);
  struct bn one;bignum_init(&one);bignum_from_int(&one, 1);
  struct bn two;bignum_init(&two);bignum_from_int(&two, 2);
  struct bn three;bignum_init(&three);bignum_from_int(&three, 3);
  struct bn four;bignum_init(&four);bignum_from_int(&four, 4);
  struct bn five;bignum_init(&five);bignum_from_int(&five, 5);
  struct bn eight;bignum_init(&eight);bignum_from_int(&eight, 8);

  while(!bignum_is_zero(&a1))//(a != 0)
  {//printf("1\n");
    bignum_mod(&a1, &two, &tmp);
    while(bignum_is_zero(&tmp))//(a/2 == 0)
    {//printf("2\n");
      bignum_div(&a1, &two, &tmp);
      bignum_assign(&a1,&tmp);//a/=2;
      bignum_mod(&n1,&eight,&r1);//int32_t r = n%8;
      //if((r==3) || (r==5))
      if((bignum_cmp(&r1,&three)==EQUAL) || (bignum_cmp(&r1,&five)==EQUAL))
        t = (-1*t);
      bignum_mod(&a1, &two, &tmp);
    }
    swap(&a1, &n1);

    //if((a%4 == 3)&&(n%4 == 3))
    bignum_mod(&a1, &four, &tmp);bignum_mod(&n1,&four,&tmp1);
    if((bignum_cmp(&tmp,&three)==EQUAL) && (bignum_cmp(&tmp1,&three)==EQUAL))
      t = (-1*t);
    bignum_mod(&a1, &n1, &tmp);
    bignum_assign(&a1, &tmp);
    //a%=n;
  }
  //printf("returning jacobi symbol\n");
  if(bignum_cmp(&n1,&one)==EQUAL)
    *jacobi1 = t;
  else
    *jacobi1 =  0;
}
