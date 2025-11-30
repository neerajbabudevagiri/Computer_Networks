#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h> 

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(1);
    }
    printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("connection with the server failed");
        exit(1);
    }
    printf("Connected to the server..\n");

    long recieved_file_size;
    if (recv(sockfd, &recieved_file_size, sizeof(long), 0) <= 0) {
        perror("Failed to receive file size");
        exit(1);
    }

    printf("Receiving file of size: %ld bytes\n", recieved_file_size);
    char *image_data = malloc(recieved_file_size);
    if (!image_data) {
        perror("malloc failed");
        exit(1);
    }

    long total_bytes_received = 0;
    while (total_bytes_received < recieved_file_size) {
        ssize_t bytes_received = recv(sockfd, image_data + total_bytes_received,
                                      recieved_file_size - total_bytes_received, 0);
        if (bytes_received <= 0) {
            perror("recv failed");
            free(image_data);
            exit(1);
        }
        total_bytes_received += bytes_received;
    }

    FILE *fp = fopen("received_sample.bmp", "wb");
    if (fp == NULL) {
        perror("File open failed");
        free(image_data);
        exit(1);
    }

    fwrite(image_data, 1, recieved_file_size, fp);
    fclose(fp);
    free(image_data);
    printf("Image received and saved as 'received_sample.bmp'\n");

    close(sockfd);
}

