#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#define SOCKET_PATH "/tmp/echo_socket"
#define BUFFER_SIZE 1024

#define ERROR -1

char* get_current_time() {
    time_t current_time;
    struct tm *local_time_info;

    current_time = time(NULL);

    local_time_info = localtime(&current_time);

    return asctime(local_time_info);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        return ERROR;
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    unlink(SOCKET_PATH); 
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind error");
        return ERROR;
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen error");
        return ERROR;
    }

    printf("Server listening on %s\n", SOCKET_PATH);

    while (1) {
        client_len = sizeof(client_addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("accept error");
            continue;
        }

        printf("Client connected\n");

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork error");
            continue;
        } else if (pid == 0) {
            // Child process
            close(server_fd);
            
            int bytes_read;
            char buffer[BUFFER_SIZE];
            while ((bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
                buffer[bytes_read] = '\0';
                printf("\nMessage: %sTime: %s\n", buffer, get_current_time());
                send(client_fd, buffer, bytes_read, 0);
            }

            if (bytes_read == -1) {
                perror("recv");
            }

            close(client_fd);
            printf("Client disconnected\n");

            exit(EXIT_SUCCESS); 
        } else {
            close(client_fd); // Close client socket in parent process
        }
    }

    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
