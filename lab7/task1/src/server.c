#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

#define PORT 5005
#define PORT_SIZE 10
#define BUF_SIZE 1024
#define TIME_SIZE 16

#define ERROR -1

char* get_current_time() {
    time_t current_time;
    struct tm *local_time_info;

    current_time = time(NULL);

    local_time_info = localtime(&current_time);

    return asctime(local_time_info);
}

int main() {
    int serv_sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    int str_len;

    serv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (serv_sock == -1) {
        perror("UDP socket creation error:");
        return ERROR;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind error:");
        return ERROR;
    }

    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        message[str_len] = '\0';
        char name[INET6_ADDRSTRLEN];
        char port[PORT_SIZE];
        if(getnameinfo((struct sockaddr*)&clnt_addr, clnt_addr_size, name, sizeof(name), port, sizeof(port), 0) != 0) {
            perror("getnameinfo error:");
            return ERROR;
        }
        char* cur_time_str = get_current_time();
        printf("Message: %sFrom: %s:%s\nTime: %s\n", message, name, port, cur_time_str);
        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);
    }

    close(serv_sock);
    return 0;
}

