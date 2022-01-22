#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sdes.h"
#include <ctype.h>
#include <stdbool.h>


char * reverse(char *buffer, int i, int j);
void swap(char *x, char *y);
char * itoa(int num, char* str, int base);

// ################ MAIN DRIVER FUNCTION #####################
int main(){



    //declar variables
    char input[11];
    char text[9] = {0};
    char * key;
    char * pt;
    char * hash;
    char * sk1;
    char * sk2;
    char fName[100] = "";
    key = malloc (sizeof (char) * 11);
    hash = malloc (sizeof (char) * 11);
    sk1 = malloc (sizeof (char) * 9);
    sk2 = malloc (sizeof (char) * 9);
    pt = malloc (sizeof (char) * 9);
    char option;
    FILE *fRead;
    FILE *fWrite;
    char ch;
    unsigned chu;
    






    // INTRO AND GET USER INPUT
    printf("\n\n-------------- Welcome to S-DES ---------------\n\n ");
    // scanf("%c", &option);
    printf("Key is hardcoded to be 1010101010\n");
	// fgets(input, 11, stdin);
    strcpy(key, "1010101010\0");
    strcpy(pt, text);
    




        //open files
        fRead = fopen("plaintext.txt", "r");
        if(fRead==NULL)
            {
                printf("\"plaintext.txt\" NOT FOUND!\n\n");
                exit(1);
            }
        fWrite = fopen("hash.txt", "w+");

            //loop each char, turn to binary, encrypt 
            printf("Generating hash. . .\n");
            chu = fgetc(fRead);
            ch = chu;
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

                //encryption
                encryption(key, pt);
                strcpy(hash, pt);
                // printf("tmp: %s\n", hash);

                strcpy(pt, "");
                chu = fgetc(fRead);
                ch = chu;
            }
                printf("Done!\n\n");
                printf("Results in 'hash.txt'\n");

            printf("Hash of file: %s\n", hash);
            fputs(hash, fWrite);




    free(key);
    free(sk1);
    free(sk2);
    free(pt);
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