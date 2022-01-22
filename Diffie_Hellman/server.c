/****************************************************
*
*    Basic minimal socket server program for use
*    in CSc 487 final projects.  You will have to
*    enhance this for your projects!!
*
*                                  RSF    11/14/20
*
****************************************************/
#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<stdlib.h>	// for system & others
#include "difh.h"
#include "sdes.h"
#include "fmea.h"

int main(int argc , char *argv[])
{
	int socket_desc , new_socket , c, read_size, i;
	struct sockaddr_in server , client;
	char *message, client_message[100], client_reply[100];

	char *list;
	list = "ls -l\n";

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8421 );                 // Random high (assumed unused) port

	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf(" unable to bind\n");
		return 1;
	}
	printf(" socket bound, ready for and waiting on a client\n");

	//Listen
	listen(socket_desc , 3);
	

	//Accept incoming connection
	printf(" Waiting for incoming connections... \n");


	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}

	printf("Connection accepted\n");


	//Reply to the client
	//(me)message = "You have located Server X at our undisclosed location.  What would you like to say?\n";
	//write(new_socket , message , strlen(message));

	//Diffie helman
	int public_info[3];//= {gen,prime,public};
	int gen = 0, prime = 0, client_public_key = 0;

	recv(new_socket, &gen , sizeof(gen), 0); // get public thing , generator and prime
	recv(new_socket, &prime , sizeof(prime), 0); // get public thing , generator and prime
	recv(new_socket, &client_public_key , sizeof(client_public_key), 0); // get public thing , generator and prime

	int priv_key= gen_private_key(); // make up my own secret number
	priv_key =  priv_key * 67 % 200;

	int shared_key;
    shared_key = calc_key(gen, priv_key,prime); // calculate shared thing by raising public thing to my secret # mod prime
	send(new_socket, &shared_key, sizeof(shared_key), 0);

	shared_key= calc_key(client_public_key, priv_key, prime);
	// call sdes* using shared secret as the 10 bit key

	// recv(new_socket , client_message ,100 , 0) // receive initial thing

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

	while(strncmp(client_message,"b",1))      // quit on "b" for "bye"
	{
	    	//Receive a message from client
		if( (read_size = recv(new_socket, client_reply , 100 , 0)) < 0)
		{
			printf("recv failed");
		}
		printf("Client says: %.*s\n\n", read_size,client_reply);
		printf("Decrytping...\n\n");

		//decrypt message
		for(int i = 0; i < read_size; i++){
				ch = client_reply[i];
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
				client_reply[i] = strtol(ct, 0, 2);
				strcpy(ct, "");
		}

		//print and read
		printf("Done!\nClient said: %.*s\n\n", read_size,client_reply);
		//write message
		printf("Reply to client: \n");
		memset(client_message,'\0',100);
		//scanf("%s",client_message);
		fgets(client_message, 100, stdin);
		printf("Encrypting message...\n\n");

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
		if( send(new_socket , &client_message, strlen(client_message) , 0) < 0)
		{
			printf("Send failed");
			return 1;
		}

		//print send
		printf("\nDone! Sending Message: %.*s\n", (int) strlen(client_message),client_message);
		close(socket_desc);
	}













	// while( (read_size = recv(new_socket , client_message , 100 , 0)) > 0 )
	// {

	// 	printf("\n Client sent %2i byte message:  %.*s\n",read_size, read_size ,client_message);

	// 	if(!strncmp(client_message,"showMe",6))
	// 	{
	// 		printf("\nFiles in this directory: \n");
	// 		system(list);
	// 		printf("\n\n");
	// 	}
	// 	//Send the message back to client
	// 	for(i=0;i< read_size;i++)
	// 	{
	// 		if ( i%2)
	// 			client_message[i] = 'z';
	// 	}

    //            	printf(" Sending back Z'd up message:  %.*s \n", read_size ,client_message);

	// 	//write(new_socket, client_message , strlen(client_message));
	// 	write(new_socket, client_message , read_size);
	// }

	// if(read_size == 0)
	// {
	// 	printf("client disconnected\n");
	// 	fflush(stdout);
	// }
	// else if(read_size == -1)
	// {
	// 	perror("receive failed");
	// }

	// //Free the socket pointer
	// close(socket_desc);
	return 0;
}
