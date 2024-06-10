#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

#define ERROR -1

int main(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket error:");
        return ERROR;
    }
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton error");
        return ERROR;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect error");
        return ERROR;
    } 

    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if(send(sock, buffer, strlen(buffer), 0) == -1) {
            perror("send error");
            break;
        }

        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv error");
            break;
        }
        else if(bytes_received == 0) {
            printf("Server closed connection\n");
            break;
        }
        else{
            buffer[bytes_received] = '\0';
            printf("Server response: %s\n", buffer);
        }

    }
    close(sock);
    return 0;
}