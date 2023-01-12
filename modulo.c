#include<stdio.h>
#include"bn.h"
void modulo(struct bn* base, struct bn* exp,struct bn* mod, struct bn* eulersCritereon)
{
    struct bn x   ;bignum_init(&x)   ;bignum_from_int(&x, 1);
    struct bn y   ;bignum_init(&y)   ;bignum_assign(&y, base);
    struct bn tmp ;bignum_init(&tmp) ;bignum_from_int(&tmp, 0);
    struct bn one  ;bignum_init(&one)  ;bignum_from_int(&one, 1);
    struct bn two  ;bignum_init(&two)  ;bignum_from_int(&two, 2);

    while(!bignum_is_zero(exp))//while (exponent > 0)
    {
        bignum_mod(exp, &two, &tmp);
        if(bignum_cmp(&tmp, &one)==EQUAL)//if (exponent % 2 == 1)
        {
            bignum_mul(&x,&y,&tmp);
            bignum_mod(&tmp, mod, &x);
        }

        bignum_mul(&y,&y,&tmp);
        bignum_mod(&tmp, mod, &y);

        //exponent = exponent / 2;
        bignum_div(exp, &two, &tmp);
        bignum_assign(exp, &tmp);
    }
    bignum_mod(&x, mod, &tmp);
    //final = bignum_to_int(&temp);
    bignum_assign(eulersCritereon, &tmp);
}
