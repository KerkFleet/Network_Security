#include "tools.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdes.h"
#include "tools.h"


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

void hash(char * inFile, char * outFile){
    char * tempKey;
    tempKey = malloc (sizeof (char) * 11);

    char text[9] = {0};
    char * pt;
    pt = malloc (sizeof (char) * 9);
    char input[11];
    int counter=0;
    char * IV;
    char * hash;
    char ch;
    unsigned chu;

    hash = malloc (sizeof (char) * 9);
    IV = malloc(sizeof(char)*9);

    FILE * fRead;
    FILE * fWrite;


    strcpy(tempKey, "1010101010\0");
    strcpy(pt, text);

    fRead = fopen(inFile, "r");
            if(fRead==NULL)
                {
                    printf("\"%s\" NOT FOUND!\n\n", inFile);
                    exit(1);
                }
    strcpy(IV, "1110001110\0");
    fWrite = fopen(outFile, "w+");
    printf("Generating hash. . .\n");
    chu = fgetc(fRead);
    ch = chu;

        //loop each char, turn to binary, encrypt 
        while(ch != EOF)
        {
            //change char to binary
            itoa(chu, text, 2);
            int len = strlen(text);

            //pad string if too short
            while(len < 8){
                if(ch > 127 && strlen(text) == 7)
                    strcat(pt, "1");
                else
                    strcat(pt, "0");
                len++;
            }
            strcat(pt, text);

            //CBC pre- encryption Xor
            if (counter ==0 ){
                counter++;
                
                for (int i=0; i<9; i++){       
                        pt[i] = xor(IV[i],pt[i]); // Xor initial value with first 8 bits of plain text 
                    }
            }

            else {
                for (int i=0; i<9; i++){       
                    pt[i] = xor(pt[i], hash[i]); // xor n'th 8 bits of the plain text with the (n-1)'th 8 bits of encrypted plain text 
                }
            }
            //encryption
            encryption(tempKey, pt);

            //print to file
            strcpy(hash,pt); // populate hash to contain the (n-1)'th 8-bits  
            strcpy(pt, "");
            chu = fgetc(fRead);
            ch = chu;
        }
            printf("Hash of fie: %s\n", hash);
            fputs(hash, fWrite);
            printf("Done!\n\n");
            printf("Results in %s\n", outFile);

}