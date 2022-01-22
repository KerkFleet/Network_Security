#ifndef SDES_H_
#define SDES_H_

    //perform the P10 operation
    void p10(char * key);

    //perform the P8 operation
    void p8(char * key, char * sk);

    //perform the shift operation
    void shift(char * key, int shift_amnt);

    //perform the ip operation
    void ip(char * plaintext);

    //perform the expansion/perutations operation
    void expan_permut(char * text);

    //perfrom ip inverse
    void ip_inverse(char * text);

    //perform fk operation
    void f_k(char * subkey, char * plaintext);

    //perform the switch operation
    void sw(char * text);

    //generate sukeys based on the key
    void gen_subkeys(char * key, char * k1, char * k2);

    //encrypt data
    char encrypt(char * key, char * plaintext);

    //decrypt data
    char decrypt(char * key, char * ciphertext);

    //convert binary string to decimal integer
    int binString_to_decInt(char * binString);

    //perform F portion of fk function
    void F(char * subkey, char * text);

    //perfrom p4 function of fk
    void p4(char * text);

    //change the S box integer back to a binary string
    void decInt_to_binString(int n, char * binArray);

    //xor to chars that are either 1 or 0
    char xor(char x, char y);

#endif 