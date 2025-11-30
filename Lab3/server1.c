#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
  

  
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 

    FILE  *fp = fopen("sample1.bmp","rb");
    fseek(fp,0,SEEK_END);
    long file_size = ftell(fp);
    fseek(fp,0,SEEK_SET);

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 
  
    send(connfd, &file_size, sizeof(long), 0);
    
    char buffer[4096];
    size_t bytes_read;
    
    while((bytes_read = fread(buffer, 1, sizeof(buffer), fp))>0){
	    send(connfd, buffer, bytes_read, 0);
    }
    fclose(fp);

    close(sockfd); 
}
