#include "sdes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int s0[4][4] = {
                {1, 0, 3, 2},
                {3, 2, 1, 0},
                {0, 2, 1, 3},
                {3, 1, 3, 2}
};
int s1[4][4] = {
                {0, 1, 2, 3},
                {2, 0, 1, 3},
                {3, 0, 1, 0},
                {2, 1, 0, 3}
};

int subkeys = 0;

void gen_subkeys(char * key, char * sk1, char * sk2){
    char * tmp_key;
    tmp_key = malloc (sizeof (char) * 11);
    strcpy(tmp_key, key);
    p10(tmp_key);
    shift(tmp_key, 1);
    p8(tmp_key, sk1);
    shift(tmp_key, 2);
    p8(tmp_key, sk2);
    free(tmp_key);
}

void p10(char * key){
    char tmp_key[11] = { key[2], key[4], key[1], key[6], key[3],
                         key[9], key[0], key[8],  key[7],  key[5], '\0' };
    strcpy(key, tmp_key);
}

void p8(char * key, char * sk){
    char tmp_key[9] = { key[5], key[2], key[6], key[3], key[7],
                        key[4], key[9], key[8], '\0'};
    strcpy(sk, tmp_key);

}

void shift(char * key, int shift_amnt){
    char tmp_key_fin[11];
    char tmp_key_first[6];
    char tmp_key_second[6];
    char stor1[shift_amnt+1];
    char stor2[shift_amnt+1];
    
    int j = 0;
    for(int i = 0; i < 11; i++){
        if(i < 5)   
            tmp_key_first[i] = key[i];
        else{
            tmp_key_second[j] = key[i];
            j++;
        }
        
    }
    tmp_key_first[5] = '\0';

    for(int i = 0; i < shift_amnt; i++){
        stor1[i] = tmp_key_first[i];
        stor2[i] = tmp_key_second[i];
    }
    char * f = tmp_key_first;
    char * s = tmp_key_second;

    for(int i = 0; i < shift_amnt; i++){
        f++;
        s++;
    }
        stor1[shift_amnt] = '\0';
        stor2[shift_amnt] = '\0';

    strcpy(tmp_key_fin, f);
    strcat(tmp_key_fin, stor1);
    strcat(tmp_key_fin, s);
    strcat(tmp_key_fin, stor2);

    strcpy(key, tmp_key_fin);
    

}

void ip(char * plaintext){
    char tmp_plaintext[9] = { plaintext[1], plaintext[5], plaintext[2],
                              plaintext[0], plaintext[3], plaintext[7], 
                              plaintext[4], plaintext[6], '\0'};
    strcpy(plaintext, tmp_plaintext);
}

void ip_inverse(char * text){
    char tmp_text[9] = { text[3], text[0], text[2],
                         text[4], text[6], text[1], 
                         text[7], text[5], '\0'};
    strcpy(text, tmp_text);

}

void f_k(char * subkey, char * plaintext){
    char * tmp;
    char new[5];
    char right[5] = {plaintext[4], plaintext[5], plaintext[6], plaintext[7], '\0'};
    tmp = malloc (sizeof (char) * 9);
    strcpy(tmp, right);
    expan_permut(tmp);
    F(subkey, tmp);
    for(int i = 0; i < 5; i++)
        new[i] = xor(plaintext[i], tmp[i]);
    for(int i = 0; i < 4; i++)
        plaintext[i] = new[i];
    free(tmp);
}

void F(char * subkey, char * text){
    char * s0_r; 
    char * s0_c;
    char * s1_r;
    char * s1_c;
    char * s0_out;
    char * s1_out;
    s0_out = malloc (sizeof (char) * 3);
    s1_out = malloc (sizeof (char) * 3);
    s0_r = malloc (sizeof (char) * 3);
    s0_c = malloc (sizeof (char) * 3);
    s1_r = malloc (sizeof (char) * 3);
    s1_c = malloc (sizeof (char) * 3);
    char F_final[5];
    int s0tmp, s1tmp;
    s0_r[0] = xor(text[0], subkey[0]); //xor n4 && sk11
    s0_r[1] = xor(text[3], subkey[3]); //xor n3 && sk14
    s0_c[0] = xor(text[1], subkey[1]); //xor n1 && sk12
    s0_c[1] = xor(text[2], subkey[2]); //xor n2 && sk13
    s1_r[0] = xor(text[4], subkey[4]); //xor n2 && sk15
    s1_r[1] = xor(text[7], subkey[7]); //xor n1 && sk18
    s1_c[0] = xor(text[5], subkey[5]); //xor n3 && sk16
    s1_c[1] = xor(text[6], subkey[6]); //xor n4 && sk17

    s0tmp = s0[binString_to_decInt(s0_r)][binString_to_decInt(s0_c)];
    s1tmp = s1[binString_to_decInt(s1_r)][binString_to_decInt(s1_c)];
    decInt_to_binString(s0tmp, s0_out);
    decInt_to_binString(s1tmp, s1_out);

    strcpy(text, s0_out);
    strcat(text, s1_out);

    p4(text);
    free(s0_out);
    free(s1_out);
    free(s0_r);
    free(s0_c);
    free(s1_r);
    free(s1_c);
}

char xor(char x, char y){
    if((x == '1' && y == '1') | (x == '0' && y == '0'))
        return '0';
    else if((x == '1' && y == '0') | (x == '0' && y == '1'))
        return '1';
}

void p4(char * text){
    char tmp[5] = {text[1], text[3], text[2], text[0], '\0'};
    strcpy(text, tmp);
}

//convert integer to binary array of chars
void decInt_to_binString(int n, char * binArray){
    char tmp[3];
    if(n==3){
        tmp[0] = '1';
        tmp[1] = '1';
    }
    else if(n==2){
        tmp[0] = '1';
        tmp[1] = '0';
    }
    else if(n==1){
        tmp[0] = '0';
        tmp[1] = '1';
    }
    else if(n==0){
        tmp[0] = '0';
        tmp[1] = '0';
    }
    tmp[2] = '\0';
    strcpy(binArray, tmp);

}
//convert binary array of chars to one integer
int binString_to_decInt(char * binString){

    int size = 0;
    size = (sizeof(binString)/4);
    int dec = 0;

    for(int i = size-1, j = 0; i >= 0; i--, j++){
        dec += ((binString[i] - '0') == 1 ? ((int)(pow(2,j) + 0.5)) : 0);
    }
    return(dec);
}


void expan_permut(char * right_ep){
    char ep[9] = {right_ep[3], right_ep[0], right_ep[1], right_ep[2],
               right_ep[1], right_ep[2], right_ep[3], right_ep[0], '\0' };
    strcpy(right_ep, ep);
}

void sw(char * text){
    char firsthalf[5] = { text[0], text[1], text[2], text[3], '\0'};
    char secondhalf[5] = { text[4], text[5], text[6], text[7], '\0'};
    strcpy(text, secondhalf);
    strcat(text, firsthalf);
}

char encryption(char * key, char * plaintext){
    char * sk1;
    char * sk2;
    sk1 = malloc (sizeof (char) * 9);
    sk2 = malloc (sizeof (char) * 9);
    gen_subkeys(key, sk1, sk2);
    ip(plaintext);
    f_k(sk1, plaintext);
    sw(plaintext);
    f_k(sk2, plaintext);
    ip_inverse(plaintext);
    free(sk1);
    free(sk2);
}

char decryption(char * key, char * ciphertext){
    char * sk1;
    char * sk2;
    sk1 = malloc (sizeof (char) * 9);
    sk2 = malloc (sizeof (char) * 9);
    gen_subkeys(key, sk1, sk2);
    ip(ciphertext);
    f_k(sk2, ciphertext);
    sw(ciphertext);
    f_k(sk1, ciphertext);
    ip_inverse(ciphertext);
    free(sk1);
    free(sk2);
}
