#DIFFIE HELLMAN COMMUNICATION

This allows for a diffie hellman key exchange to make a secure shared key.

The user can then send messages that are encrypted using S-DES and the secret shared key.


You can compile each client and server using:
* gcc client.c difh.c fmea.c sdes.c -o client -lm
* gcc server.c difh.c fmea.c sdes.c -o server -lm

Then execute the server using: ./server

followed by the client using: ./client

From there you can begin sending messages back and forth!
