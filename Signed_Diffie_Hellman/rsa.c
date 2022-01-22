#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"
#include "fmea.h"


int rsa_decrypt(long int msg, long int n, long int d){
	return(fme(d, msg, n));
}
int rsa_encrypt(long int msg, long int n, long int e){
	return(fme(e, msg, n));
}

long int modInverse(long int a, long int m)
{
	long int x, y, res, g;
		g = gcdExtended(a, m, &x, &y);
		if (g != 1){
			printf("Inverse doesn't exist!\n");
			res = 0;
		}
		else
		{
			// m is added to handle negative x
			res = (x % m + m) % m;
			printf("Modular multiplicative of %ld mod %ld inverse is %ld\n", a, m, res);
		}

	return res;
}

long int gcdExtended(long int a, long int b, long int* x, long int* y)
{
	if (a == 0)
	{
		*x = 0, *y = 1;
		return b;
	}

	long int x1, y1; // To store results of recursive call
	long int gcd = gcdExtended(b % a, a, &x1, &y1);

	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

long int calc_private_key(long int p, long int q, long int e){
	long int totient = (p-1)*(q-1);
	long int d = modInverse(e, totient);
	return d;
}
// Driver Code


void gen_private_public_pair(long int * public, long int * private, long int * n){
    long int x, totient, inverse, tmp, d=0, p, q, e;
    printf("Enter two large primes separated by spaces: ");
    scanf("%ld %ld", &p, &q);
	char grbg;
	scanf("%c", &grbg);
	// Function call
	int less = p < q ? p : q;

    srand(time(0));
    srand(time(0));
    while(d == 0){
        e = rand() % less;
        printf("e: %ld\n", e);
        d = calc_private_key(p, q, e);
        if(d == 0)
            printf("Trying again...\n");
    }
	*public = e;
	*private = d;
	*n = p * q;

}