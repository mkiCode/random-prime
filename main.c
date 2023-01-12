
#include<stdio.h>
#include<stdint.h>
#include<windows.h>
#include<Bcrypt.h>
#include <stdbool.h>
#include<time.h>

#define RNG BCRYPT_RNG_ALGORITHM

#include"jacobi.h"
#include"modulo.h"
#include"bn.h"
#include"preprocessorOptions.h"

bool solovayStrassen(struct bn* num, uint64_t accuracy)
{

  #ifdef RECORD_TIME
  clock_t start = clock();
  clock_t current;
  double dur;
  #endif

  BCRYPT_ALG_HANDLE alg;
  BCryptOpenAlgorithmProvider(&alg,RNG,MS_PRIMITIVE_PROVIDER,0);
  uint64_t k = accuracy;
  struct bn a1; bignum_init(&a1); bignum_from_int(&a1, 0);
  struct bn n1; bignum_init(&n1);bignum_assign(&n1, num);
  int8_t jacobi1 = 99;
  struct bn eulersCritereon1; bignum_init(&eulersCritereon1);
  struct bn temp1; bignum_init(&temp1);
  struct bn temp2;bignum_init(&temp2);
  struct bn temp3;bignum_init(&temp3);
  struct bn two;bignum_init(&two); bignum_from_int(&two, 2);
  int32_t isPrime;

  while(k--)
  {
    BCryptGenRandom(alg, (PUCHAR)a1.array, BYTESIZE,0);
    while((bignum_cmp(&a1, &two)<=EQUAL)||(bignum_cmp(&n1, &a1)<=EQUAL))
      BCryptGenRandom(alg, (PUCHAR)a1.array,BYTESIZE,0);

    #ifdef RECORD_TIME
    current = clock();
    dur = (current-start)/CLOCKS_PER_SEC;
    printf("after rand gen:%f\n", dur);
    #endif

    jacobiSymbol(&a1,&n1, &jacobi1);

    #ifdef RECORD_TIME
    current = clock();
    dur = (current-start)/CLOCKS_PER_SEC;
    printf("after jacobi:%f\n", dur);
    #endif


    bignum_assign(&temp1, &n1);bignum_dec(&temp1);bignum_div(&temp1, &two, &temp2);
    modulo(&a1,&temp2,&n1,&eulersCritereon1);

    #ifdef RECORD_TIME
    current = clock();
    dur = (current-start)/CLOCKS_PER_SEC;
    printf("after modulo:%f\n", dur);
    #endif

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
    if(!isPrime )
      return 0;
  }

  return 1;

}
void main(void)//add argument for byte size
{

  int stringsize = 3000;
  BCRYPT_ALG_HANDLE alg;
  BCryptOpenAlgorithmProvider(&alg,RNG,MS_PRIMITIVE_PROVIDER,0);

  //FILE OUTPUT SETUP could define check this as a compile time option
  char outputFile[] = "prime0.txt";
  FILE *output = fopen(outputFile, "r");
  while(output &&(outputFile[5]<='9'))
    outputFile[5]++;
  if((outputFile[5]<'9'))
    outputFile[5] = '0';
  fclose(output);
  output = fopen(outputFile, "w");
  //END FILE OUTPUT SETUP

  char string1[stringsize];

  #ifdef RECORD_TIME
  clock_t t1 = clock();
  clock_t t2 = clock();
  clock_t t3 = clock();
  clock_t t4 = clock();
  double dur;
  #endif

  struct bn num;bignum_init(&num);

  do
  {
    #ifdef RECORD_TIME
    t4 = clock();
    dur = (t4-t3)/CLOCKS_PER_SEC;
    printf("check time:%f\n", dur);
    t3 = clock();
    #endif

    do
      BCryptGenRandom(alg, (PUCHAR)num.array,BYTESIZE,0);
    while(!((*((uint32_t*)num.array))%2));

    bignum_to_string(&num, string1, stringsize);
    printf("checking bignum:%s\n", string1);
  }
  while(!solovayStrassen(&num, 10));

  bignum_to_string(&num, string1, stringsize);
  printf("Random Prime bignum:%s\n", string1);

  BCryptCloseAlgorithmProvider(alg,0);

  #ifdef RECORD_TIME
  t2 = clock();
  dur = (t2-t1)/CLOCKS_PER_SEC;
  printf("total time:%f\n", dur);
  #endif

  //OUTPUT NUM TO FILE
  fwrite(string1,sizeof(char),stringsize,output);
  fclose(output);
  //OUPTUT
}

