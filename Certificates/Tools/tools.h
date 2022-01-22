#ifndef TOOLS_H_
#define TOOLS_H_
#include <stdbool.h>
#include <time.h>

char* itoa   (int value, char * buffer, int base);
void  swap   (char *x, char *y);
char* reverse(char *buffer, int i, int j);
char* hash   (char * inFile, char * outFile, bool append);
void  create_keys  (void);
void  create_cert  (void);
int  validate_cert(char *);
void  CRL_populate (void);
void  CRL_validate (void);
void  tree_validate(void);
void get_keys(char * filename, int * public_key);
void write_file(char * cert_name, int sockfd);
void get_file(char * filename);
void send_file(int sockfd);

#endif