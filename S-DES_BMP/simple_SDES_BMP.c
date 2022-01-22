// Headers
//Nathan Dobbs
//Some sources for BMPS
//http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
//https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file/38440684
//^useful stackexchange thread that got me started on reading the bmp and fixing some weird errors//
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <bitset>
#include <math.h>
#include <time.h>
#include "tools.h"


// Main function
int main()
{
    char * inFile, * outFile;
    inFile = malloc (sizeof (char) * 50);
    outFile = malloc (sizeof (char) * 50);
    strcpy(inFile, "plaintext.txt");
    strcpy(outFile, "hash.txt");
    hash(inFile, outFile);

}
