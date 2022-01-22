#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"
#include "fmea/fmea.h"


int rsa_decrypt(long int msg, long int p, long int q, long int e, long int d){
	return(fme(d, msg, p*q));
}
int rsa_encrypt(long int msg, long int p, long int q, long int e){
	return(fme(e, msg, p*q));
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