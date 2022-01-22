#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"
#include <time.h>


int main()
{
    long int n, x, totient, inverse, tmp, d, p, q, e;
    printf("Enter two large primes separated by spaces: ");
    scanf("%ld %ld", &p, &q);
    long int msg;
    printf("Enter the value you would like to encrypt:");
    scanf("%ld", &msg);
	// Function call
	int less = p < q ? p : q;

    srand(time(0));
    srand(time(0));
    e = rand() % less;
    d = calc_private_key(p, q, e);
    long int cypher = rsa_encrypt(msg, p, q, e);
    printf("Encrypted message: %ld\n", cypher);
    long int pt = rsa_decrypt(cypher, p, q, e, d);
    printf("Decrypted message: %ld\n", pt);
}