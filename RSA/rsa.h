#ifndef RSA_H_
#define RSA_H_

long int gcdExtended(long int a, long int b, long int* x, long int* y);
long int modInverse(long int a, long int m);
long int calc_private_key(long int p, long int q, long int e);
int rsa_encrypt(long int msg, long int p, long int q, long int e);
int rsa_decrypt(long int msg, long int p, long int q, long int e, long int d);

#endif