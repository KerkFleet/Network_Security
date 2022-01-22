#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdes.h"
#include "tools.h"
#include"rsa.h"
#include"fmea.h"
#include <stdbool.h>
#include <arpa/inet.h>
int TIMER = 15;
#define SIZE 1024

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

char * hash(char * inFile, char * outFile, bool append){
    char * tempKey;
    tempKey = malloc (sizeof (char) * 11);
    char text[9] = {0};
    char * pt;
    pt = malloc (sizeof (char) * 9);
    char input[11];
    int counter=0;
    char * IV;
    char * hash;
    char ch;
    unsigned chu;

    hash = malloc (sizeof (char) * 9);
    IV = malloc(sizeof(char)*9);

    FILE * fRead;
    FILE * fWrite;


    strcpy(tempKey, "1010101010\0");
    strcpy(pt, text);

    fRead = fopen(inFile, "r");
            if(fRead==NULL)
                {
                    printf("\"%s\" NOT FOUND!\n\n", inFile);
                    exit(1);
                }
    strcpy(IV, "1110001110\0");
    if(append == true)
        fWrite = fopen(outFile, "a");
    else
        fWrite = fopen(outFile, "w+");
    // printf("Generating hash. . .\n");
    chu = fgetc(fRead);
    ch = chu;

        //loop each char, turn to binary, encrypt 
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

            //CBC pre- encryption Xor
            if (counter ==0 ){
                counter++;
                
                for (int i=0; i<9; i++){       
                        pt[i] = xor(IV[i],pt[i]); // Xor initial value with first 8 bits of plain text 
                    }
            }

            else {
                for (int i=0; i<9; i++){       
                    pt[i] = xor(pt[i], hash[i]); // xor n'th 8 bits of the plain text with the (n-1)'th 8 bits of encrypted plain text 
                }
            }
            //encryption
            encryption(tempKey, pt);

            //print to file
            strcpy(hash,pt); // populate hash to contain the (n-1)'th 8-bits  
            strcpy(pt, "");
            chu = fgetc(fRead);
            ch = chu;
        }
            // printf("Done!\n");
            // printf("Hash of file: %s\n\n", hash);
            fputs(hash, fWrite);
            fputc('\n', fWrite);
            fclose(fWrite);
            fclose(fRead);
            return(hash);

}

void create_keys(void)
{
    long int x, totient, inverse, tmp, d=0, p, q, e, n;
    long int public, private;
    FILE * keyWrite;
    keyWrite = fopen("keys.txt", "w+");
    fputs("PUBLIC PRIVATE MOD", keyWrite);
    for(int i = 0; i < 5; i++){
        printf("Enter two large primes separated by spaces: ");
        scanf("%ld %ld", &p, &q);
        long int msg;
        printf("Enter the value you would like to encrypt:");
        scanf("%ld", &msg);
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
        public = e;
        private = d;
        n = p * q;

        fprintf(keyWrite, "%ld   %ld   %ld\n", e, d, n);
        d = 0;

    }
}
struct certificate{
    char version[15];
    char serial_num[30];
    char issuer_name[50];
    //period of validity
    int start;
    int end;
    char subject_name[50];
    //algorithm & parameters
    char algorithm[15];
    int pub_key[2];
    int trustlevel;
};
void create_cert( void) 
{
    char * inFile, * outFile;
    char * hash_val;
    char cert_file[50];
    strcpy(cert_file, "Certificates/");
    int hash_int;
    int grbg;
    long int e, d, mod;
    struct certificate cert;
    FILE * writeCert;
    inFile = malloc (sizeof (char) * 50);
    outFile = malloc (sizeof (char) * 50);

    printf("Welcome to the certificate creation station.\nPlease fill out the necessary data below.\n\n");
    while ((getchar()) != '\n');
     
    // collect user input
    printf("Subject name: ");
    fgets(cert.subject_name, 50, stdin);
    
    strcat(cert_file, cert.subject_name);
    int length = strlen(cert_file);
    cert_file[length-1]='.';
    cert_file[length]='t';
    cert_file[length+1]='x';
    cert_file[length+2]='t';
    cert_file[length+3]='\0';

    printf("CERT NAME: %s\n", cert_file);
    writeCert = fopen(cert_file, "w+");
    if(!writeCert){
        printf("Error creating certificate file!\n");
        exit(1);
    }

    printf("Version: ");
    fgets(cert.version, 15, stdin);

    printf("Serial number: ");
    fgets(cert.serial_num, 30, stdin);

    printf("Issuer name: ");
    fgets(cert.issuer_name, 50, stdin);

    printf("Start date: ");
    scanf("%d", &cert.start);

    printf("End date: ");
    scanf("%d", &cert.end);
    getchar();

    printf("Algorithm: ");
    fgets(cert.algorithm, 15, stdin);

    printf("Public key(e): ");
    scanf("%d", &cert.pub_key[0]);

    printf("Public key(n): ");
    scanf("%d", &cert.pub_key[1]);

    printf("Trust Level: ");
    scanf("%d", &cert.trustlevel);

    getchar();
    
    fprintf(writeCert, "%s%s%s%d\n%d\n%s%s%d\n%d\n%d\n", cert.version, cert.serial_num, cert.issuer_name, 
            cert.start, cert.end, cert.subject_name, cert.algorithm, cert.pub_key[0], cert.pub_key[1], cert.trustlevel);    
    
    fclose(writeCert);


    strcpy(inFile, cert_file);
    strcpy(outFile, "hash.txt");
    hash_val = hash(inFile, outFile, false);

    hash_int = strtol(hash_val, NULL, 2);
    printf("Please enter your private key pair separated by a space to sign: ");
    scanf("%ld %ld", &d, &mod);
    int sig = rsa_encrypt(hash_int, mod, d);

    writeCert = fopen(cert_file, "a");
    fprintf(writeCert, "%d", sig);
    fclose(writeCert);

}

int validate_cert(char * filename)
{
    //verify certs
    FILE * verifyCert, * tempCert, * crl;
    char line[100];
    char * dir = "Certificates/";
    char issuer_cert[100];
    char file_path[50];
    char issuer_name[100];
    char * hash_val;
    int hash_int, start, end, signature, serial_num =0, status;
    long int e, d, mod;
  
    strcpy(issuer_cert, dir);
    strcpy(file_path, dir);
    strcat(file_path, filename);

    //open cert to verify
    verifyCert = fopen(file_path, "r+"); // without hard code it causes seg fault core dump
    if(!verifyCert){
        printf("Cert doesn't exist!\n");
        return 0;
    }
    tempCert = fopen("temp_cert.txt", "w+");

    //grab issuer name and signature while writing all but signature to a temp file
    int i = 0;
    while (i < 11)
    {
        fgets(line, 100, verifyCert);

        if(i==1)
            {serial_num = strtol(line, NULL, 10);
            printf("serial num:%d\n",serial_num);
            }

        if(i==2)
            strcpy(issuer_name, line);
    
        if(i==3)
            start = strtol(line, NULL, 10);

        if(i==4)
            end = strtol(line, NULL, 10);

        if(i!=10)
            fputs(line, tempCert);

        i++;
    }
    
    printf("Issuer: %s\n", issuer_name);
    signature = strtol(line, NULL, 10);
    fclose(tempCert);
    fclose(verifyCert);

    //hash temp file
    hash_val = hash("temp_cert.txt", "hash.txt", true);
    hash_int = strtol(hash_val, NULL, 2);

    //open issuer name cert 
    int length = strlen(issuer_name);
    issuer_name[length-1]='.';
    issuer_name[length]='t';
    issuer_name[length+1]='x';
    issuer_name[length+2]='t';
    issuer_name[length+3]='\0';
    //printf("x%sx\n",issuer_name);
    strcat(issuer_cert, issuer_name);
    verifyCert = fopen(issuer_cert, "r"); // this seg fault core dump
    if(!verifyCert){
        printf("Error opening certificate: %s!\n", issuer_cert);
        exit(1);
    }
    
    //grab issuer public key data
    i = 0;
    while (i < 11)
    {
        fgets(line, 100, verifyCert);
        if(i == 7)
            e = strtol(line, NULL, 10);
        if(i == 8)
            mod = strtol(line, NULL, 10);
        i++;
    }
    printf("Public key: (%ld, %ld)\n", e, mod);

    printf("Signed cert signature: %d\n", signature);
    //decrypt signature
    signature = rsa_decrypt(signature, mod, e);
    printf("File hash: %d\n", hash_int);
    printf("Decrypted signature: %d\n\n", signature);

    //make comparison
    if(hash_int == signature){
        printf("Hashes match.\n");
    }
    else
        printf("Hash doesn't match!\n");

    int delta = end-start;

    //check CRL list
    bool revoke = false;
    crl = fopen("Certificates/CRL.txt", "r+");
    if(!crl){
        printf("CRL file doesn't exist!\n");
        return 0;
    }
    int num = 0;
    do{
        fgets(line, 100, crl);

        if(line[0]=='~')
        { break; }
        else
        {
            num = strtol(line, NULL, 10);
            if(num == serial_num)
            {
                revoke = true;
                printf("Certificate is on the CRL list!\n");
            }
            else if(num!=serial_num)
                revoke = false;
        }
    }while(!feof(crl) && !revoke);

    if(revoke == false){
        printf("Certificate is not on CRL list.\n");
    }

    //check time
    if(delta < TIMER)
        printf("Timestamp is valid.\n");
    else
        printf("Timestamp is not valid.\n");

    //make final validation message
    if(hash_int == signature && delta<TIMER && revoke ==false )
        {
            printf("\nCertficate is valid.\n");
            return 1;
        }
    else
        {
            printf("\nCertificate is not valid.\n");
            return 0;
        }
    
    // remove temp cert
     remove("temp_cert.txt");

}

void CRL_populate(void) // needs revocation date
{
    int s_num=0;
    char line[100];
    char done = 'f';
    FILE * writeFile;
    getc(stdin);
    writeFile = fopen("Certificates/CRL.txt", "w+");

    //get user input for crl
    while(true){
        printf("(Enter 'e' to exit)\nEnter the serial number of the revoked cert: ");
        fgets(line, 100, stdin);
        done = line[0];
        if(done == 'e')
            break;
        fputs(line, writeFile);
    }
    fclose(writeFile);

    //hash cert
    int sig = strtol(hash("Certificates/CRL.txt", "hash.txt", true), NULL, 2);

    //sign with Cathy's keys and write to cert
    sig = rsa_decrypt(sig, 4819, 4489);
    writeFile = fopen("Certificates/CRL.txt", "a");
    fputs("~\n", writeFile);
    fprintf(writeFile, "%d", sig);
    fclose(writeFile);

}
void CRL_validate(void)
{
    char issuer[50];
    char line[100], sig_string[100];
    long int e, mod;
    int sig, hash_int;
    FILE * fp, * temp;
    int i = 0;
    
    //info
    printf("Issuer of CRL: Cathy\n");
    printf("Public key: (49, 4819)\n\n");
    //get signature
    fp = fopen("Certificates/CRL.txt", "r");
    if(!fp){
        printf("Error in opening CRL!\n");
    }
    temp = fopen("temp_cert.txt", "w+");
    while(fgets(line, 100, fp)){
        if(line[0] == '~'){
            fgets(line, 100, fp);
            strcpy(sig_string, line);
            break;
        }
        else{
            fputs(line, temp);
        }
    }
    fclose(fp);
    fclose(temp);
            
    hash_int = strtol(hash("temp_cert.txt", "hash.txt", false), NULL, 2);
    printf("Hash of CRL: %d\n", hash_int);
    sig = strtol(sig_string, NULL, 10);
    printf("Signature of CRL: %d\n", sig);

    sig = rsa_encrypt(sig, 4819, 49);
    printf("Decrypted Signature: %d\n", sig);
    if(hash_int == sig)
        printf("Hashes match.\nCRL is valid!\n");
    else
        printf("Hash doesn't match!\nCRL is not valid.\n");

    remove("temp_cert.txt");
}
void tree_validate(void) // dependancies: user must have their own cert, the cert they want to validate and cathy's cert on file. 
{   int status; // for validate cert 
    int trustlevel[50]; 
    char signer[50];
    char my_sig[50];
    char holder[50];
    char * dir = "Certificates/";
    char name [50];
    char trusted_name [50];
    FILE * verifyCert, *kdcert;
    printf("In tree\n");
    getchar();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// validates integrity and existance of the cert that the user wants to verify trust with
    printf("Please enter the name of the cert you would like to validate\n");
    scanf("%s",name);
    getchar();
    printf("x%sx\n",name);
    status = validate_cert(name); // validates initial file's integrity returns 1 if cert valid else 0
    while(status!=1)
    {
        printf("I'm sorry but that certificate for %s does not exist is no longer valid.\n",name);
        printf("Would you like to try a different cert name?\n                      0.)Yes / 1.)No: ");
        scanf("%d",&status);
        getchar();
        if(status==0)
        {
            printf("Please enter the name of the cert you would like to validate\n");
            scanf("%s",name);
            getchar();
            status = validate_cert(name); 
        }
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validates Cathy's cert
    printf("Please enter the name of the trusted Cert that you have on file\n");//check for cathys cert
    scanf("%s",trusted_name);
    getchar();
    status = validate_cert(trusted_name); // validates initial file's integrity returns 1 if cert valid else 0
    while(status!=1)
    {
        printf("I'm sorry but that certificate for %s does not exist is no longer valid.\n", trusted_name);
        printf("Would you like to try a different cert name?\n                   0.)Yes / 1.)No: ");
        scanf("%d", &status);
        getchar();
        if(status==0)
        {
            printf("Please enter the name of the cert you would like to validate\n");
            scanf("%s",trusted_name);
            status = validate_cert(trusted_name); 
        }
    }
   
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char file_path[50];
    char line[100];
    //open my cert and rip my signer my_sig
    printf("NAME:%s\n",name);
    strcpy(file_path, dir);
    strcat(file_path, name);

    verifyCert = fopen(file_path, "r");
    int i=0, node = 0; // to count how mant nodes on the cert tree have been traversed
    do{ i=0;
        while(i<11)
            {   
                fgets(line, 100, verifyCert); //
                if(i==2)
                    {strcpy(signer,line); // gets the signer of the cert from file
                    printf("SIGNER: %s",signer);}
                if (i==5)
                    strcpy(holder, line);   // get the cert holder name
                if(i==10)
                    trustlevel[i] = strtol(line, NULL, 10); // get the trust level
                i++;
            }
        
        int length = strlen(signer);
        signer[length-1]='.';
        signer[length]='t';
        signer[length+1]='x';
        signer[length+2]='t';
        signer[length+3]='\0';
        status = validate_cert(signer);
        strcpy(file_path, dir);
        strcat(file_path, signer);
        printf("SIGNER filepath: %s\n",file_path);
        verifyCert = fopen(file_path, "r+"); 
    
        if (status==0)
            { 
                printf("Unable to verify tree\nTree ended at %s's Cert",holder);
                return;
            }
        node++;
        }while( !strcmp(signer,trusted_name));

        int TL = trustlevel[0];
        for(int j=1; j<sizeof(trustlevel);j++) // finds thje lowest trust level of the tree
        {
            if (trustlevel[i]< TL)
            {
                TL = trustlevel[i];
            }
        }
printf("The certificate for %s can be trusted\nThe tree has been verified to %s with a lowest trust level of %d and %d nodes were traversed\n",name, trusted_name, TL,node);

}

void get_keys(char * filename, int * public_key){
    char dir[50] = "Certificates/";
    char cert_path[50];
    char line[50];
    int e, mod;
    strcpy(cert_path, dir);
    strcat(cert_path, filename);
    FILE * fp;

    fp = fopen(cert_path, "r");
    
    for(int i = 0; i < 9; i++){
        fgets(line, 100, fp);
        if(i == 7)
            e = strtol(line, NULL, 10);
        if(i == 8)
            mod = strtol(line, NULL, 10);
    }

    public_key[0] = e;
    public_key[1] = mod;

}

void write_file(char * cert_name, int sockfd){
  int n, length;
  FILE *fp;
  char buffer[SIZE];
  char filename[100];
  n = recv(sockfd, &length, sizeof(int), 0);
  n = recv(sockfd, buffer, length * sizeof(char), 0);
  strcpy(cert_name, buffer);
  strcpy(filename, "Certificates/");
  strcat(filename, cert_name);
  filename[strlen(filename)] = 0;
  printf("Received certificate: %s\n", filename);


  fp = fopen(filename, "w+");
  if(!fp){
      printf("Couldn't open file: %s\n", filename);
      exit(1);
  }
  bzero(buffer, SIZE);
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  fclose(fp);
  return;
}

void get_file(char * filename){
  char file[50];
  printf("Enter the certificate file name to send: ");

  scanf("%s", file);
  getc(stdin);
  // fgets(file, 50, stdin);
  strcpy(filename, file);
  filename[strlen(filename)] = 0;
}

void send_file(int sockfd){
  int n;
  FILE *fp;
  char *dir = "Certificates/";
  char cert_name[50];
  char filename[50];
  char data[SIZE] = {0};

  strcpy(filename, dir);
  get_file(cert_name);
  strcat(filename, cert_name);
  // printf("x%sx", filename);

  int length = strlen(cert_name);
  n = send(sockfd, &length, sizeof(int), 0);
  n = send(sockfd, cert_name, strlen(cert_name) * sizeof(char), 0);
  bzero(data, SIZE);
  if(n == -1){
      perror("[-]Error in sending file.");
      exit(1);
  }

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("[-]Error in reading file: x%sx\n", filename);
    exit(1);
  }
  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
  // for(int i = 0; i < __INT_MAX__; i++){}
}