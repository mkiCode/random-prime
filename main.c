#include<time.h>
#include<stdio.h>
//#include<stdlib.h>
#include<stdint.h>
#include<ntstatus.h>
//#include<string.h>
#include<windows.h>
#include<Bcrypt.h>
#include"bn.h"
//#include<math.h>
#define RNG BCRYPT_RNG_ALGORITHM
#include"jacobi.h"
#include"modulo.h"

#include <stdbool.h>
bool solovayStrassen(struct bn* num, uint64_t accuracy)
{
  BCRYPT_ALG_HANDLE alg;
  BCryptOpenAlgorithmProvider(&alg,RNG,MS_PRIMITIVE_PROVIDER,0);
  uint64_t k = accuracy;
  struct bn a1; bignum_init(&a1); bignum_from_int(&a1, 0);
  struct bn n1; bignum_init(&n1);bignum_assign(&n1, num);
  int8_t jacobi1 = 99; //= INT_MAX;
  struct bn eulersCritereon1; bignum_init(&eulersCritereon1);
  struct bn temp1; bignum_init(&temp1);
  struct bn temp2;bignum_init(&temp2);
  struct bn temp3;bignum_init(&temp3);
  //struct bn two;bignum_init(&two); bignum_from_int(&one, 1);
  struct bn two;bignum_init(&two); bignum_from_int(&two, 2);
  //struct bn jacobi1;bignum_init(&jacobi1);
  int32_t isPrime;

  while(k--)
  {
    //printf("1...\n");
    BCryptGenRandom(alg, (PUCHAR)a1.array, 2,0);
    //printf("random lld:%lld\n", a);

    //printf("1.2...\n");
    while((bignum_cmp(&a1, &two)<=EQUAL)||(bignum_cmp(&n1, &a1)<=EQUAL))
      BCryptGenRandom(alg, (PUCHAR)a1.array,2,0);
//    a = 3301693;
//    *(uint64_t*)a1.array = a;
    //printf("generate random a:%llu...\n", a);
    //printf("generate random bignum a:%llu...\n", *((uint64_t*)a1.array));
    //printf("random lld:%lld\n", a);
    //printf("2...\n");

    jacobiSymbol(&a1,&n1, &jacobi1);
    //printf("after jacobi\n");
    //printf("a:%llu\n", a);
    //printf("a1:%llu\n",  *((uint64_t*)a1.array));
    //printf("n:%llu\n", n);
    //printf("n1:%llu\n",  *((uint64_t*)n1.array));
    //printf("jacobi:%d\n", jacobi);
    //printf("jacobi1:%d\n", jacobi1);
    //printf("jacobiSymbol(a:%lld,n:%lld):%lld\n", a, n, jacobi);
    //printf("3...\n");
    bignum_assign(&temp1, &n1);bignum_dec(&temp1);bignum_div(&temp1, &two, &temp2);
    modulo(&a1,&temp2,&n1,&eulersCritereon1);
    //printf("after modulo\n");
    //printf("a:%llu\n", a);
    //printf("a1:%llu\n",  *((uint64_t*)a1.array));
    //printf("n:%llu\n", n);
    //printf("n1:%llu\n",  *((uint64_t*)n1.array));
    //printf("eulers:%llu...\n", eulersCritereon);
    //printf("eulsers bignum:%llu...\n", *((uint64_t*)eulersCritereon1.array));

    //printf("modulo(a:%llu,(n-1)/2:%llu,n:%llu):%llu\n", a, n, n, eulersCritereon);
    //printf("4...\n");
    bignum_assign(&temp1, &n1);bignum_dec(&temp1);
    bignum_mod(&temp1, &n1, &temp2);bignum_assign(&temp1, &temp2);
    bignum_assign(&temp2, &n1);bignum_inc(&temp2);
    bignum_mod(&temp2, &n1, &temp3);bignum_assign(&temp2, &temp3);
    
    switch(jacobi1)
    {
      case -1:
        isPrime = (bignum_cmp(&temp1, &eulersCritereon1) == EQUAL);
        break;
      case 0:
        isPrime = bignum_is_zero(&eulersCritereon1);
        break;
      case 1:
        isPrime = (bignum_cmp(&temp2, &eulersCritereon1) == EQUAL);
        break;
      default:
        printf("unexpected jacobi result\n");
    }
    //printf("5...\n");
    //printf("((n+jacobi)mod n == eulersCritereon):%lld\n", isPrime);
    if(!isPrime )
      return 0;
  }

  return 1;

}
void main(void)//add argument for byte size
{
  char string1[3000];
  clock_t t1 = clock();

  BCRYPT_ALG_HANDLE alg;
  //uint64_t number = 0;
  struct bn num;bignum_init(&num);//bignum_from_int(&num, 0);
  BCryptOpenAlgorithmProvider(&alg,RNG,MS_PRIMITIVE_PROVIDER,0);
  do
  {
//    number = 0;
    //BCryptGenRandom(alg, (PUCHAR)&number, 3,0);
    BCryptGenRandom(alg, (PUCHAR)num.array,2,0);
//    number = *((uint64_t*)num.array);
    printf("checking num:%llu...\n", bignum_to_int(&num));
    bignum_to_string(&num, string1, 3000);
    printf("checking bignum:%s\n", string1);
  }
  while(!solovayStrassen(&num, 10));
  bignum_to_string(&num, string1, 1000);
  printf("Random Prime bignum:%s\n", string1);
  //printf("Random Prime:%llu\n", *((uint64_t*)num.array));
  BCryptCloseAlgorithmProvider(alg,0);
  clock_t t2 = clock();
  double dur = (t2-t1)/CLOCKS_PER_SEC;
  printf("time:%f\n", dur);
}

