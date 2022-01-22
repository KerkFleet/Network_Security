#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../Tools/tools.h"
#include "../../Tools/difh.h"
#include "../../Tools/rsa.h"
#include "../../Tools/sdes.h"
#include "../../Tools/fmea.h"
#define SIZE 1024
char filename[SIZE];
char cert_name[50];


int main(){
  char *ip = "0.0.0.0";
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  long int n, d;
  printf("Who are you? (Enter your private key pair separated by a space)\n");
  scanf("%ld %ld", &d, &n);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");
  if(listen(sockfd, 10) == 0){
      printf("[+]Listening....\n");
  }else{
      perror("[-]Error in listening");
      exit(1);
  }
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

  write_file(cert_name, new_sock);
  printf("[+]Certificate received successfully.\n\n\n");

  if(validate_cert(cert_name) == 0){
    printf("Could not validate certificate. Ending communication.\n");
  }
  // tree_validate();
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

  //cert is valid, get dh params
  int gen = 0, prime = 0, client_public_key = 0;
	int public_key[2];
  

  recv(new_sock, &gen, sizeof(gen), 0);
  recv(new_sock, &prime , sizeof(prime), 0); // get public thing , generator and prime
	recv(new_sock, &client_public_key , sizeof(client_public_key), 0); // get public thing , generator and prime

  printf("Generating diffie-hellman secret key. . .\n");
	//get public key from cert
	get_keys(cert_name, public_key);

	//decrypt dh params
	gen = rsa_decrypt(gen, public_key[1], public_key[0]);
	prime = rsa_decrypt(prime, public_key[1], public_key[0]);
	client_public_key = rsa_decrypt(client_public_key, public_key[1], public_key[0]);

	int priv_key= gen_private_key(); // make up my own secret number
	priv_key =  priv_key * 67 % 200;

	int shared_key, send_key;
  shared_key = calc_key(gen, priv_key,prime); // calculate shared thing by raising public thing to my secret # mod prime

	//encrypt with cient pub priv key
  printf("Encrypting with senders public key -- n = %d and e = %d\n", public_key[1], public_key[0]);
	send_key = rsa_encrypt(shared_key, public_key[1], public_key[0]);
	//sign with my own pub priv key
  printf("Encrypting with my private key -- n = %ld and d = %ld\n", n, d);
	send_key = rsa_encrypt(send_key, n, d);
	printf("Encrypted: %d\n", send_key);

  printf("Done! Now sending the encrypted shared key and my certificate. . .\n");
  send_file(new_sock);
  printf("Certificate sent successfully!\n");
  close(new_sock);

  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  
	send(new_sock, &send_key, sizeof(send_key), 0);


	printf("Shared key: %d\n", shared_key);
  printf("Secure, connection established.\n");


  /// DO PASSWORD STUFF
  int q = 23, n_pass = 47, a = 5, k = 3;
  char * salt = "abcdefg";
  char password[100] = "password";
  FILE * pwFile;
  char * password_hash;
  int v;
  int A, B;

  //hash password and salt
  strcat(password, salt);
  pwFile = fopen("psTemp.txt", "w+");
  fputs(password, pwFile);
  fclose(pwFile);
  password_hash = hash("psTemp.txt", "hash.txt", true);
  remove("psTemp.txt");

  //calc parameter B to send to client
  int x = strtol(password_hash, NULL, 2);
  v = fme(x, a, n_pass);
  time_t t;
  srand(t);
  int nonce = rand() % n_pass;
  B = k * v + fme(nonce, a, n_pass);
  printf("B: %d\n", B);

  //send B, recv A
  recv(new_sock, &A , sizeof(A), 0); // get public thing , generator and prime
  send(new_sock, &B , sizeof(B), 0); // get public thing , generator and prime
  printf("A: %d\n", A);

  printf("Made it!\n");
  //calc u = hash(A, B)
  char AB[40];
  char temp_B[20];
  char temp_A[20];
  char * AB_hash;
  itoa(B, temp_B, 10);
  itoa(A, temp_A, 10);
  strcpy(AB, temp_A);
  strcat(AB, temp_B);
  pwFile = fopen("psTemp.txt", "w+");
  fputs(AB, pwFile);
  fclose(pwFile);
  AB_hash = hash("psTemp.txt", "hash.txt", true);
  remove("psTemp.txt");
  int u = strtol(AB_hash, NULL, 2);


  int term1 = A * fme(u, v, n_pass);
  int Ss = fme(nonce, term1, n_pass);
  printf("Server secret: %d\n", Ss);

  //recv client M1 = H(A|B|Sc)
  int M1;
  recv(new_sock, &M1, sizeof(M1), 0); // get public thing , generator and prime
  printf("Received M1: %d\n", M1);

  //compare to M2 H(A|B|Ss)
  char before_hash[80]; 
  char Ss_string[20];
  int M2;
  itoa(Ss, Ss_string, 10);
  strcpy(before_hash, AB);
  strcat(before_hash, Ss_string);
  pwFile = fopen("psTemp.txt", "w+");
  fputs(before_hash, pwFile);
  fclose(pwFile);
  char * hash_val;
  hash_val = hash("psTemp.txt", "hash.txt", true);
  M2 = strtol(hash_val, NULL, 2);
  remove("psTemp.txt");
  printf("Hash(A|B|Ss): %d\n", M2);

  if(M1 == M2)
    printf("Hashes match! Password accepted.\n");
  else{
    printf("Password is incorrect. . . Closing connection.\n");
    close(new_sock);
    exit(1);
  }

  //calc hash(A|M1|Ss)
  char response_hash[100];
  char temp_M1[100];
  char temp_Ss[100];
  int read_size;
  char client_message[100], client_reply[100];
  strcpy(response_hash, temp_A);
  itoa(M1, temp_M1, 10);
  itoa(Ss, temp_Ss, 10);
  strcat(response_hash, temp_M1);
  strcat(response_hash, temp_Ss);
  M2 = strtol(response_hash, NULL, 10);
  printf("Sending M2 - Hash(A|M1|Ss): %d\n", M2);

	send(new_sock, &M2, sizeof(M2), 0);

  //we have shared_key variable for key to use for s-des 
  //Everything checks out, allow the receiving of commands
  printf("Waiting on commands from client. . .\n");

  //probably want to loop
	// recv(new_sock, command, strlen(command), 0);
  //




  char text[9] = {0};
	unsigned char ch;
	//pt = plaintext, ct = ciphertext
	char * pt, * ct, * sdes_key, * temp_key;
	sdes_key = malloc (sizeof (char) * 11);
	temp_key = malloc (sizeof (char) * 64);
	pt = malloc (sizeof (char) * 9);
	ct = malloc (sizeof (char) * 9);
  strcpy(pt, "");
  strcpy(ct, "");

	//change shared int key to 10 bit binary
	itoa(shared_key, temp_key, 2);
	strncpy(sdes_key, temp_key, 10);
  while(strlen(sdes_key) < 10)
      strcat(sdes_key, "0");


  printf("SDES KEY: %s\n", sdes_key);
while(strncmp(client_message,"b",1))      // quit on "b" for "bye"
	{
    strcpy(client_message, "");
	    	//Receive a message from client
		if( (read_size = recv(new_sock, client_reply , strlen(client_reply) , 0)) < 0)
		{
			printf("recv failed");
		}
    client_reply[strlen(client_reply)] = 0;
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
				decryption(sdes_key, ct);
				client_reply[i] = strtol(ct, 0, 2);
				strcpy(ct, "");
		}

		//print and read
		printf("Done!\nClient said: %.*s\n\n", read_size,client_reply);
		//write message
		printf("Replying to client... \n");
		//scanf("%s",client_message);
    FILE * fp;
    fp = popen(client_reply, "r");
		fgets(client_message, 100, fp);
    client_message[strlen(client_message)] = 0;
    printf("Client msg: %s\n", client_message);
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
		if( send(new_sock , client_message, strlen(client_message) , 0) < 0)
		{
			printf("Send failed");
			return 1;
		}

		//print send
		printf("\nDone! Sending Message: %s\n", client_message);
	}




  remove("hash.txt");



close(new_sock);
  return 0;
}
