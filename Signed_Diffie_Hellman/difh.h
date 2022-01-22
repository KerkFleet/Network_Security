
#ifndef DIFH_H_
#define DIFH_H_

int gen_private_key();
void gen_public_info(int * generator, int * prime);
int calc_key(int gen, int key, int prime);

#endif