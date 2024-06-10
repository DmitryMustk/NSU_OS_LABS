#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

#define ERROR -1

char* get_current_time() {
    time_t current_time;
    struct tm *local_time_info;

    current_time = time(NULL);

    local_time_info = localtime(&current_time);

    return asctime(local_time_info);
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("\nMessage: %sTime: %s", buffer, get_current_time());
        send(client_socket, buffer, bytes_received, 0);
    }

    close(client_socket);
    exit(0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket error:");
        return ERROR;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return ERROR;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen error");
        return ERROR;
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept error");
            continue;
        }

        printf("New connection, forked process\n");

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // Child process
            close(server_fd);
            handle_client(client_fd);
            exit(EXIT_SUCCESS);
        } else {
            close(client_fd); 
        }
    }

    return 0;
}
