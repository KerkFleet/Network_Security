#include <time.h>
#include <stdlib.h>
#include "difh.h"
#include "fmea.h"


int gen_private_key(){
    srand(time(0));
    int key = rand() % 200 + 100;
    return key;
}


void gen_public_info(int * generator, int * prime){
    srand(time(0));
    int num = rand() % 10;
    int primes[10] =         {101, 227, 521, 149, 239, 751, 293, 199, 991, 11};
    int generators[10] =     {93,  5,   17,  13,  7,   28,  19,  39,  23,  8} ;

    *prime = primes[num];
    *generator = generators[num];
}


int calc_key(int gen, int key, int prime){
    int num = fme(key, gen, prime);
    return num;
}

