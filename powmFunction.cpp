#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

int powm(int base, int exp, int mod){

    int r = 1;
    int i;

    for(i=0; i<exp; i++){
        r *= base;
        r %= mod;
    }

    return r;   
}

int main(void){
    int base;
    int exp;
    int mod;
    int result;

    printf("base: ");
    scanf("%d", &exp);

    printf("exp: ");
    scanf("%d", &exp);

    printf("mod: ");
    scanf("%d", &mod);

    result = powm(base, exp, mod);
    printf("result: %d /n", result);


    return 0;

    
}