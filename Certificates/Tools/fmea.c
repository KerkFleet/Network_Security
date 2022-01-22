
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"
#include "fmea.h"

int fme(unsigned long long int power, unsigned long long int base, unsigned long long int mod){
    char * buffer;
    long num = base;
    buffer = malloc (sizeof (char) * 64);
    itoa(power, buffer, 2);

    for(int i = 0; i < strlen(buffer)-1; i++){
        if(buffer[i+1] == '0'){
            num = (pow(num,2));
            num = num % mod;
        }
        else if(buffer[i+1] == '1'){
            num = ((pow(num,2)) * base);
            num = num % mod;
        }
    }

    return num;

}
