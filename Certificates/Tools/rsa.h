#ifndef RSA_H_
#define RSA_H_

//void gen_private_public_pair(long int * public, long int * private, long int * n);
long int gcdExtended(long int a, long int b, long int* x, long int* y);
long int modInverse(long int a, long int m);
long int calc_private_key(long int p, long int q, long int e);
int rsa_decrypt(long int msg, long int n, long int d);
int rsa_encrypt(long int msg, long int n, long int e);

#endif