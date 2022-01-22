#include "fmea.h"
#include <stdio.h>

int main(){
    int power = 147;
    int base = 3;
    int mod = 257;
    long int num = 0;

    num = fme(power, base, mod);
    printf("%d^%d mod %d  ==  %ld mod %d\n", base, power, mod, num, mod);

}
