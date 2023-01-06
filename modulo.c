#include<stdio.h>
#include<bn.h>
uint64_t modulo(uint64_t Base, uint64_t exponent, uint64_t mod)
{
    uint64_t final;
    char string1[1000];
    //char string2[1000];
    //char string3[1000];

    uint64_t x = 1;struct bn x1;bignum_init(&x1);bignum_from_int(&x1, x);
    //bignum_to_string(&x1, string1, 1000);
    //printf("x1:%s\n", string1);

    uint64_t y = Base;struct bn y1;bignum_init(&y1);bignum_from_int(&y1, y);

    struct bn temp;bignum_init(&temp);bignum_from_int(&temp, 0);

    struct bn mod1;bignum_init(&mod1);bignum_from_int(&mod1, mod);


    //bignum_to_string(&x1, string1, 1000);
    //printf("x1:%s\n", string1);
    while (exponent > 0)
    {
        //printf("run_loop\n");
        //printf("x:%llu, y:%llu exp:%llu\n", x, y, exponent);
        //printf("hang1?\n");
        if (exponent % 2 == 1)
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
        exponent = exponent / 2;
        //printf("hang5?\n");

    }
    //printf("endloop\n");

    bignum_mod(&x1, &mod1, &temp);
    //bignum_to_string(&temp, string1, 1000);
    //printf("modulo return:%s)\n", string1);
    final = bignum_to_int(&temp);
    //printf("final mod:%llu\n", final);
    return final;//bignum_to_int(&temp);
}
