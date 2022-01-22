#include "fmea.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

long int fme(long int power, long int base, long int mod){
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

//change char to integer string
char * itoa(int value, char * buffer, int base){
       // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1); 
}
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}
