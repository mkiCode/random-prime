#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<ntstatus.h>
#include<string.h>
#include<windows.h>
#include<Bcrypt.h>
#include<math.h>
#define RNG BCRYPT_RNG_ALGORITHM
#include"jacobi.h"
#include"modulo.h"
#include <stdbool.h>
bool solovayStrassen(uint64_t number, uint64_t accuracy)
{
  BCRYPT_ALG_HANDLE alg;
  BCryptOpenAlgorithmProvider(&alg,RNG,MS_PRIMITIVE_PROVIDER,0);
  uint64_t k = accuracy;
  uint64_t a = 0;
  uint64_t n = number;
  int32_t  jacobi = INT_MAX;
  uint64_t eulersCritereon;
  uint64_t isPrime;

  while(k--)
  {
    //3 bytes for a appropriate testing size
    BCryptGenRandom(alg, (PUCHAR)&a, 8,0);
    //printf("random lld:%lld\n", a);
    while(((a)<=2)||((n-1)<=a))
      BCryptGenRandom(alg, (PUCHAR)&a, 8,0);
    //printf("random lld:%lld\n", a);
    jacobi = jacobiSymbol(a,n);
    //printf("jacobiSymbol(a:%lld,n:%lld):%lld\n", a, n, jacobi);
    eulersCritereon = modulo(a,(n-1)/2,n);
    //printf("modulo(a:%llu,(n-1)/2:%llu,n:%llu):%llu\n", a, n, n, eulersCritereon);
    switch(jacobi)
    {
      case -1:
        isPrime = (((n - 1)%n) == eulersCritereon);
        break;
      case 0:
        isPrime = (((n + 0)%n) == eulersCritereon);
        break;
      case 1:
        isPrime = (((n + 1)%n) == eulersCritereon);
        break;
      default:
        printf("unexpected jacobi result\n");
    }

    //printf("((n+jacobi)mod n == eulersCritereon):%lld\n", isPrime);
    if(!isPrime )
      return 0;
  }
  return 1;

}
void main(void)
{
  BCRYPT_ALG_HANDLE alg;
  uint64_t number = 0;
  BCryptOpenAlgorithmProvider(&alg,RNG,MS_PRIMITIVE_PROVIDER,0);
  do
  {
    number = 0;
    BCryptGenRandom(alg, (PUCHAR)&number, 8,0);
    //printf("sizeof(unsigned long long):%llu\n", sizeof(unsigned long long));
    //printf("ULLONG_MAX:%llu\n", ULLONG_MAX);
    //printf("checking num:%llu...\n", number);
  }
  while(!solovayStrassen(number, 10));

  printf("Random Prime:%llu\n", number);
  BCryptCloseAlgorithmProvider(alg,0);

}
