# S-DES BMP ENCRYPTION

### About

---

This program takes a bmp image file, and encrypts or decrypts it using s-des in either 
electronic code book or cipher block chain mode, and outputs the encrypted/decrypted
data into the specified output file

### Execution

---

To execute this program:

    * Compile with: gcc simple_SDES_BMP.c sdes.c tools.c -lm
    * Execute with: ./a.out inputfile.bmp outputfile.bmp
    * Encrypt/Decrypt, ECB/CBC modes, the key are specified during runtime
