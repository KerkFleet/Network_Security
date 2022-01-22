#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sdes.h"
#include "fmea.h"

int sum(int x, int y);

int main(){

int x = 5;
int y = 2;

int z = sum(x, y);
printf("Sum is: %d", z);



}

int sum(int x, int y){
	return x + y;
}