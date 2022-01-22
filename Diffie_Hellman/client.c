/****************************************************
*
*    Basic minimal socket client program for use
*    in CSc 487 final projects.  You will have to
*    enhance this for your projects!!
*
*                                  RSF    11/14/20
*
****************************************************/
#include<stdio.h>      // used printf/scanf for demo (puts/getchar would be leaner)
#include<stdlib.h>
#include<string.h>	
#include<sys/socket.h>
#include<arpa/inet.h>  // for inet_addr and sockaddr_in structs
#include "difh.h"
#include "sdes.h"
#include "fmea.h"

int main(int argc , char *argv[])
{
	int socket_desc;    // file descripter returned by socket command
	int read_size;
	struct sockaddr_in server;    // in arpa/inet.h
	char  server_reply[100], client_message[100];   // will need to be bigger
	char ip[15];
	strcpy(ip, "0.0.0.0");
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	printf("Trying to create socket\n");
	if (socket_desc == -1)
	{
		printf("Unable to create socket\n");
	}
		
// *********** This is the line you need to edit ****************
	server.sin_addr.s_addr = inet_addr(ip);  // doesn't like localhost?
	server.sin_family = AF_INET;
	server.sin_port = htons( 8421 );    // random "high"  port number

	// Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf(" connect error\n");
		return 1;
	}
	

	printf("Setting up secure connection with %s...\n", ip);

	// generate key and public info
	int generator, prime, priv_key, shared_key, client_public_key, server_public_key=0;
	int public_info[3];
	priv_key = gen_private_key();
	gen_public_info(&generator, &prime);

	//calculate public key
	client_public_key = calc_key(generator, priv_key, prime);
	
	//store for sending
	// public_info[0] = generator;
	// public_info[1] = prime;
	// public_info[2] = client_public_key;
	

	//send public info to server
	send(socket_desc , &generator, sizeof(generator), 0); 
	send(socket_desc , &prime, sizeof(prime), 0);
	send(socket_desc , &client_public_key, sizeof(client_public_key), 0);

	//Receive a reply from the server (comes in the same format as was sent)
	if( (read_size = recv(socket_desc, &server_public_key , sizeof(server_public_key), 0)) < 0)
	{
		printf("recv failed");
	}


	//Calculate a shared key for future encryptions
	shared_key = calc_key(server_public_key, priv_key, prime);
	
	//create temp variables for encrypting/decrypting
	char text[9] = {0};
	unsigned char ch;
	//pt = plaintext, ct = ciphertext
	char * pt, * ct, * sdes_key, * temp_key;
	sdes_key = malloc (sizeof (char) * 11);
	temp_key = malloc (sizeof (char) * 64);
	pt = malloc (sizeof (char) * 9);
	ct = malloc (sizeof (char) * 9);

	//change shared int key to 10 bit binary
	itoa(shared_key, temp_key, 2);
	strncpy(sdes_key, temp_key, 10);


	//can now use S-DES with shared key for encrypting/decrypting messages
	//Get data from keyboard and send  to server
	printf("What do you want to send to the server. (b for bye)\n");

	while(strncmp(client_message,"b",1))      // quit on "b" for "bye"
	{
		//write message
		memset(client_message,'\0',100);
		//scanf("%s",client_message);
		fgets(client_message, 100, stdin);

		//encrypt message
		for(int i = 0; i < strlen(client_message); i++){
				ch = client_message[i];
                itoa(ch, text, 2);
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
				encryption(sdes_key, pt);
				client_message[i] = strtol(pt, 0, 2);
				strcpy(pt, "");
		}

		//send message
		if( send(socket_desc , &client_message, strlen(client_message) , 0) < 0)
		{
			printf("Send failed");
			return 1;
		}

		//print send
		printf("\nSending Message: %.*s\n", (int) strlen(client_message),client_message);
	
		//Receive a reply from the server
		if( (read_size = recv(socket_desc, server_reply , 100 , 0)) < 0)
		{
			printf("recv failed");
		}
		printf("Server says: %.*s\n\n", read_size,server_reply);
		printf("Decrytping...\n\n");

		//decrypt message
		for(int i = 0; i < read_size; i++){
				ch = server_reply[i];
                itoa(ch, text, 2);
                int len = strlen(text);

                //pad string if too short
                while(len < 8){
                    if(ch > 127 && strlen(text) == 7)
                        strcat(ct, "1");
                    else
                        strcat(ct, "0");
                    len++;
                }
                strcat(ct, text);
				decrypt(sdes_key, ct);
				server_reply[i] = strtol(ct, 0, 2);
				strcpy(ct, "");
		}

		//print and read
		printf("Done!\nServer says: %.*s\n\n", read_size,server_reply);
		printf("Reply to Server(b for bye):\n");
	}

	
	return 0;
}
