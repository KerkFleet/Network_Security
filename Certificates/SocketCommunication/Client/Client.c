#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include "../../Tools/tools.h"
#include "../../Tools/difh.h"
#include "../../Tools/sdes.h"
#include "../../Tools/rsa.h"
#include "../../Tools/fmea.h"
#define SIZE 1024
char cert_name[50];

void reconnenct(int sockfd, char * ip){
}



int main(){
  char *ip = "0.0.0.0";
  int port = 8080;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;

  long int n, d;
  printf("Who are you? (Enter your private key pair separated by a space)\n");
  scanf("%ld %ld", &d, &n);
  getchar();


  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
      if(e == -1) {
        perror("[-]Error in socket");
        exit(1);
      }
      printf("[+]Connected to Server.\n");
      send_file(sockfd);
      
      printf("[+]Certificate sent successfully.\n");
      close(sockfd);


      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
      }

      server_addr.sin_family = AF_INET;
      server_addr.sin_port = port;
      server_addr.sin_addr.s_addr = inet_addr(ip);

        e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
          if(e == -1) {
            perror("[-]Error in socket");
            exit(1);
          }

      
      int generator, prime, priv_key, shared_key, client_public_key, server_public_key=0;
        int public_info[3];
        int public_key[2];

        //gen dh params

        priv_key= gen_private_key(); // make up my own secret number
        priv_key =  priv_key * 43 % 200;

        gen_public_info(&generator, &prime);

        //calculate public key
        client_public_key = calc_key(generator, priv_key, prime);

        //sign dh params
        generator = rsa_encrypt(generator, n, d);
        prime = rsa_encrypt(prime, n, d);
        client_public_key = rsa_encrypt(client_public_key, n, d);

          //send cert and signed DH params
        printf("Sending signed dh parameters. . .\n");
        
        send(sockfd, &generator, sizeof(generator), 0);
        send(sockfd, &prime, sizeof(prime), 0);
        send(sockfd, &client_public_key, sizeof(client_public_key), 0);
        printf("Done!\n");
        printf("Awaiting response. . .\n");


        //receive cert back, validate, and get keys
        write_file(cert_name, sockfd);
        printf("Certificate received: %s\n", cert_name);
        printf("Valiating cert. . .\n");
        close(sockfd);
        if(validate_cert(cert_name) == 0){
          printf("Could not vaildate certificate. Ending communication.\n");
          exit(1);
        }
        for(int i = 0; i < __INT_MAX__; i++){}

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0) {
          perror("[-]Error in socket");
          exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = port;
        server_addr.sin_addr.s_addr = inet_addr(ip);

          e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
            if(e == -1) {
              perror("[-]Error in socket");
              exit(1);
            }

        

        printf("Waiting. . .\n");
        //receive shared key, encrypted and signed 
        recv(sockfd, &shared_key , sizeof(shared_key), 0); // get public thing , generator and prime
        get_keys(cert_name, public_key);
        
        printf("Encrypted: %d\n", shared_key);
        printf("Decrypting with sender's public key -- n = %d and e = %d\n", public_key[1], public_key[0]);
        shared_key = rsa_decrypt(shared_key, public_key[1], public_key[0]);
        printf("Decrypting with my private key -- n = %ld and d = %ld\n", n, d);
        shared_key = rsa_decrypt(shared_key, n, d);

        printf("Shared key: %d\n", shared_key);
        printf("Secure, connection established.\n");
      
        /// DO PASSWORD STUFF
        int q = 23, n_pass = 47, a = 5, k = 3;
        int B, A;
        char * salt = "abcdefg";
        char password[100];
        FILE * pwFile;
        char * password_hash;

        //hash password and salt
        printf("Please enter your password: ");
        scanf("%s", password);
        getchar();
        strcat(password, salt);
        pwFile = fopen("psTemp.txt", "w+");
        fputs(password, pwFile);
        fclose(pwFile);
        password_hash = hash("psTemp.txt", "hash.txt", true);
        remove("psTemp.txt");

        //calc A to send to serverr
        int x = strtol(password_hash, NULL, 2);

        time_t t;
        srand(t);
        int nonce = rand() % n_pass;
        A = fme(nonce, a, n_pass);
        printf("A: %d\n", A);

        //send A, recv B
        send(sockfd, &A , sizeof(A), 0); // get public thing , generator and prime
        recv(sockfd, &B , sizeof(B), 0); // get public thing , generator and prime
        printf("B: %d\n", B);


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

        //calc client secret
        int term1 = B-k*fme(x, a, n_pass);
        printf("X: %d\n", x);
        int term2 = nonce + u * x;
        int Sc = fme(term2, term1, n_pass);
        printf("Client secret: %d\n", Sc);

        //calc M1 = Hash(A|B|Sc)
        char before_hash[80];
        char Sc_string[80];
        int M1;
        itoa(Sc, Sc_string, 10);
        strcpy(before_hash, AB);
        strcat(before_hash, Sc_string);
        pwFile = fopen("psTemp.txt", "w+");
        fputs(before_hash, pwFile);
        fclose(pwFile);
        char * hash_val;
        hash_val = hash("psTemp.txt", "hash.txt", true);
        M1 = strtol(hash_val, NULL, 2); // get hash value as int
        remove("psTemp.txt");
        //send M1 to prove we know the password
        printf("Sent M1 - Hash(A|B|Sc): %d\n", M1);
        send(sockfd, &M1, sizeof(M1), 0); 

        //receive and verify M2
        int M2;
        recv(sockfd, &M2, sizeof(M2), 0); 
        printf("Received M2: %d\n", M2);
        // verify hash against hash(A|M1|Sc)
        char hash_check[100];
        char temp_M1[100];
        strcpy(hash_check, temp_A);
        itoa(M1, temp_M1, 10);
        strcat(hash_check, temp_M1);
        strcat(hash_check, Sc_string);
        M1 = strtol(hash_check, NULL, 10);
        printf("Hash (A|M1|Sc): %d\n", M1);

        if(M1 == M2)
          printf("Server response validated, password accepted!\n");
        else{
          printf("Password denied. Closing connection.\n");
          close(sockfd);
          exit(1);
        }


     
  //we have shared_key variable for key to use for s-des 

//create temp variables for encrypting/decrypting
	char text[9] = {0};
	unsigned char ch;
  char client_message[100], server_reply[1000];
  int read_size;
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

	//can now use S-DES with shared key for encrypting/decrypting messages
	//Get data from keyboard and send  to server
	printf("What do you want to send to the server. (b for bye)\n");

	while(client_message[0] != 'b')      // quit on "b" for "bye"
	{
		//write message
    char command[30];
    strcpy(client_message, "");
    strcpy(command, "");
		// scanf("%s",client_message);
    // getc(stdin);
		fgets(client_message, 100, stdin);
    client_message[strlen(client_message)] = 0;
    strncpy(command, client_message, 30);
    command[strlen(command)] = 0;

		//encrypt message
		for(int i = 0; i < strlen(command); i++){
				ch = command[i];
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
        printf("PT: %s\n", pt);
				command[i] = strtol(pt, 0, 2);
				strcpy(pt, "");
		}

		//send message
		if(send(sockfd , command, strlen(command) , 0) < 0)
		{
			printf("Send failed");
			return 1;
		}

		//print send
		printf("\nSending Message: %.*s\n", (int)strlen(command),command);
	
		//Receive a reply from the server
		if( (read_size = recv(sockfd, server_reply , 1000, 0)) < 0)
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
				decryption(sdes_key, ct);
				server_reply[i] = strtol(ct, 0, 2);
				strcpy(ct, "");
		}

		//print and read
		printf("Done!\nServer says: %.*s\n\n", read_size,server_reply);
		printf("Reply to Server(b for bye):\n");
	}








      close(sockfd);
  return 0;
}
