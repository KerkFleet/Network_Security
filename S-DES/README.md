# S-DES Project
By: Shelby Kerkvliet
Date: 9/21/2021

---

This project consists of a header file and source code for S-DES encryption and decryption routines.

The S-DES expects data in the form of a 10 bit binary character array for the key, and an 8 bit binary character array for plaintext/ciphertext to be encoded/decoded.

The driver program assumes there to be a file named 'plaintext.txt' in the same directory, which contains the data to be encrypted. It will create a file named 'ciphertext.txt' which it will write its results to. If decrypting, it will assume a file name of 'ciphertext.txt', and write to 'plaintext.txt'

Sample files have been provided.


### To run this program:
compile using:
    * gcc -o a.out main_encryption.c sdes.c -lm
run using the command:
    * ./a.out