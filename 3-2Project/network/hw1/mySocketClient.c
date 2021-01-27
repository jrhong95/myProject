#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <string.h> 

#define MAX_BUFFER 1024
int main(int argc, char const *argv[]) 
{
    int sock = 0; 
    struct sockaddr_in client_addr; 
    char buffer[MAX_BUFFER] = {0}; 

    if(argc != 3){
		printf("Usage : %s [IP] [PORT]\n", argv[0]);
		exit(1);
	}

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket create error"); 
        exit(EXIT_FAILURE);
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(argv[1]);
    client_addr.sin_port = htons(atoi(argv[2])); 
   
    if (connect(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0){ 
        perror("Connection failed"); 
        exit(EXIT_FAILURE);
    } 

    while(1){
        printf("input: ");
        scanf("%s", &buffer);
        send(sock, buffer, MAX_BUFFER, 0);
        if(strcmp(buffer, "exit") == 0)
			break;
    }

    close(sock);
    return 0; 
} 