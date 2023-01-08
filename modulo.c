#include<stdio.h>
#include"bn.h"
void modulo(struct bn* Base1,struct bn* exponent1,struct bn* mod2, struct bn* eulersCritereon1)
{
    //printf("exponent:%llu\n", exponent);
    //printf("exponent:%llu\n", *((uint64_t*)exponent1->array));
    //uint64_t final;
    //char string2[1000];
    //char string3[1000];

    struct bn x1;bignum_init(&x1);bignum_from_int(&x1, 1);
    //bignum_to_string(&x1, string1, 1000);
    //printf("x1:%s\n", string1);

    struct bn y1;bignum_init(&y1);bignum_assign(&y1, Base1);

    struct bn temp;bignum_init(&temp);bignum_from_int(&temp, 0);

    struct bn mod1;bignum_init(&mod1);bignum_assign(&mod1, mod2);
    struct bn zero;bignum_init(&zero);bignum_from_int(&zero, 0);
    struct bn one;bignum_init(&one);bignum_from_int(&one, 1);
    struct bn two;bignum_init(&two);bignum_from_int(&two, 2);

    //bignum_to_string(&x1, string1, 1000);
    //printf("x1:%s\n", string1);
    while(bignum_cmp(exponent1,&zero)==LARGER)//while (exponent > 0)
    {
    //printf("1exponent:%llu\n", *((uint64_t*)exponent1->array));

        //printf("run_loop\n");
        //printf("x:%llu, y:%llu exp:%llu\n", x, y, exponent);
        //printf("hang1?\n");
        bignum_mod(exponent1, &two, &temp);
        if(bignum_cmp(&temp, &one)==EQUAL)//if (exponent % 2 == 1)
        {
            //bignum_to_string(&x1, string1, 1000);
            //printf("x1:%s\n", string1);
            //printf("hang2?\n");
            //printf("(x:%llu * y:%llu) %c mod:%llu = ", x, y, '%', mod);
            //x = (x * y) % mod;
            //printf("x:%llu)\n", x);
            //bignum_to_string(&x1, string1, 1000);
            //bignum_to_string(&y1, string2, 1000);
            //printf("bignum_mul(&x1:%s, &y1:%s, &temp);\n", string1, string2);

            bignum_mul(&x1,&y1,&temp);
            //printf("hang3?\n");
            //bignum_to_string(&temp, string1, 1000);
            //bignum_to_string(&mod1, string2, 1000);
            //printf("bignum_mod(&temp:%s, &mod1:%s,", string1, string2);
            bignum_mod(&temp, &mod1, &x1);
            //bignum_to_string(&x1, string3, 1000);
            //printf("x1:%s);\n", string3);
        }
        //y = (y * y) % mod;
        //printf("square y\n");
        bignum_mul(&y1,&y1,&temp);
        //printf("finished\n");
        bignum_mod(&temp, &mod1, &y1);
        //printf("hang4?\n");
        //exponent = exponent / 2;
        bignum_div(exponent1, &two, &temp);
        bignum_assign(exponent1, &temp);
        //printf("hang5?\n");
        //printf("2exponent:%llu\n", *((uint64_t*)exponent1->array));
        //bignum_to_string(exponent1, string1, 1000);
        //printf("2bignum exponent:%s\n", string1);
       // bignum_to_string(&zero, string1, 1000);
        //printf("2bignum zero:%s\n", string1);

        //printf("bignum_cmp(exponent1,&zero)==LARGER:%d\n", bignum_cmp(exponent1,&zero)==LARGER);
    }
    //printf("endloop\n");

    bignum_mod(&x1, &mod1, &temp);
    //bignum_to_string(&temp, string1, 1000);
    //printf("modulo return:%s)\n", string1);
//    final = bignum_to_int(&temp);
    //printf("final mod:%llu\n", final);
    bignum_assign(eulersCritereon1, &temp);
    //return final;//bignum_to_int(&temp);
}
