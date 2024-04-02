#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 4096
#define PORT 5005

#define ERROR -1

int main(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        perror("Create socket error:");
        return ERROR;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    char message[BUF_SIZE];
    ssize_t str_len;
    printf("Welcome to UDP echo server. Type your message or /q to quit\n");
    while(1) {
        printf("Client: ");
        fgets(message, BUF_SIZE, stdin);
        if(strcmp(message, "/q\n") == 0) {
            printf("Goodbye!\n");
            break;
        }
        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        str_len = recvfrom(sock, message, BUF_SIZE, 0, NULL, NULL);
        message[str_len] = '\0';
        printf("Server: %s\n", message);
    }

    close(sock);
    return 0;
}
