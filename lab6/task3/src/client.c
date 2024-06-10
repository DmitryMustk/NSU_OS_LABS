#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/echo_socket"
#define BUFFER_SIZE 1024

#define ERROR -1

int main() {
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        return ERROR;
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        return ERROR;
    }

    printf("Connected to server\n");

    // Communication with server
    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send data to server
        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("send error");
            break;
        }

        // Receive data from server
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv error");
            break;
        } else if (bytes_received == 0) {
            printf("Server closed connection\n");
            break;
        } else {
            buffer[bytes_received] = '\0';
            printf("Server response: %s", buffer);
        }
    }

    close(client_fd);

    return 0;
}
