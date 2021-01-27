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
    int server_fd, sock, msg_size; 
    struct sockaddr_in server_addr; 
    int server_addr_len = sizeof(server_addr); 
	int opt = 1;
    char buffer[MAX_BUFFER] = {0};
    
	if(argc < 3){
		printf("Usage : %s [IP] [PORT]\n", argv[0]);
		return -1;
	}
	
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) { 
        perror("setsockopt failed"); 
        exit(EXIT_FAILURE);
    }

	memset(&server_addr, 0, server_addr_len);
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    server_addr.sin_port = htons(atoi(argv[2])); 
    
    if (bind(server_fd, (struct sockaddr *)&server_addr, server_addr_len) < 0 ) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
	
    if (listen(server_fd, 3) < 0) { 
        perror("listen failed"); 
        exit(EXIT_FAILURE); 
    }
	else
		printf("Wait Client...\n");

    if ((sock = accept(server_fd, (struct sockaddr *)&server_addr,
	 (socklen_t*)&server_addr_len)) < 0) { 
        perror("accept failed"); 
        exit(EXIT_FAILURE); 
    }
	else
		printf("Client connected.\n");
	
	while(1){
		msg_size = recv(sock , buffer, MAX_BUFFER, 0);
		if(strcmp(buffer, "exit") == 0)
			break;
		printf(">> %s\n", buffer);
	}

	printf("Server exit...\n");
	close(sock);
	close(server_fd);
    return 0; 
}