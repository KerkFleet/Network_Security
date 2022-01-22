# Signed_Diffie_Hellman

### About

This code performs a secure communication over sockets utilizing signed diffie hellman key exchange.

### To execute code:
  * Please specify the IP of the server in client.c
  * Compile client.c using 'gcc client.c difh.c fmea.c rsa.c sdes.c -o client -lm'
  * Compile server.c using 'gcc server.c difh.c fmea.c rsa.c sdes.c -o server -lm'
