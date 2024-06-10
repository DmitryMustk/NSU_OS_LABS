#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define PORT 8080 
#define MAX_CLIENTS 5
#define BUFFER_SIZE 4096

char* get_current_time() {
    time_t current_time;
    struct tm *local_time_info;

    current_time = time(NULL);

    local_time_info = localtime(&current_time);

    return asctime(local_time_info);
}

void send_html_response(int client_socket) {
    char response[] = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                      "<!DOCTYPE html>\r\n"
                      "<html><head><title>lab7/task3</title>"
                      "<style>"
                      "body {"
                      "    margin: 0;"
                      "    overflow: hidden;"
                      "}"
                      "#canvas {"
                      "    display: block;"
                      "    background-color: #161616ff;"
                      "}"
                      "</style>"
                      "<script>"
                      "console.log('Script loaded');"
                      "document.addEventListener('DOMContentLoaded', function() {"
                      "    const canvas = document.getElementById('canvas');"
                      "    const ctx = canvas.getContext('2d');"
                      "    const circles = [];"
                      "    const colors = ['#FF6633', '#FFB399', '#FF33FF', '#FFFF99', '#00B3E6',"
                      "                    '#E6B333', '#3366E6', '#999966', '#99FF99', '#B34D4D',"
                      "                    '#80B300', '#809900', '#E6B3B3', '#6680B3', '#66991A',"
                      "                    '#FF99E6', '#CCFF1A', '#FF1A66', '#E6331A', '#33FFCC',"
                      "                    '#66994D', '#B366CC', '#4D8000', '#B33300', '#CC80CC',"
                      "                    '#66664D', '#991AFF', '#E666FF', '#4DB3FF', '#1AB399',"
                      "                    '#E666B3', '#33991A', '#CC9999', '#B3B31A', '#00E680',"
                      "                    '#4D8066', '#809980', '#E6FF80', '#1AFF33', '#999933',"
                      "                    '#FF3380', '#CCCC00', '#66E64D', '#4D80CC', '#9900B3',"
                      "                    '#E64D66', '#4DB380', '#FF4D4D', '#99E6E6', '#6666FF'];"
                      "    const numCircles = 100;"
                      "    const maxSpeed = 10;"
                      "    const minRadius = 5;"
                      "    const maxRadius = 50;"
                      "    const gravity = 0.02;"
                      "    class Circle {"
                      "        constructor(x, y, radius, dx, dy, color) {"
                      "            this.x = x;"
                      "            this.y = y;"
                      "            this.radius = radius;"
                      "            this.dx = dx;"
                      "            this.dy = dy;"
                      "            this.color = color;"
                      "        }"
                      "        draw() {"
                      "            ctx.beginPath();"
                      "            ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);"
                      "            ctx.fillStyle = this.color;"
                      "            ctx.fill();"
                      "            ctx.closePath();"
                      "        }"
                      "        update() {"
                      "            if (this.y + this.radius + this.dy > canvas.height) {"
                      "                this.dy = -this.dy * 0.8;"
                      "            } else {"
                      "                this.dy += gravity;"
                      "            }"
                      "            if (this.x + this.radius + this.dx > canvas.width || this.x - this.radius <= 0) {"
                      "                this.dx = -this.dx;"
                      "            }"
                      "            this.x += this.dx;"
                      "            this.y += this.dy;"
                      "            this.draw();"
                      "        }"
                      "    }"
                      "    function init() {"
                      "        canvas.width = window.innerWidth;"
                      "        canvas.height = window.innerHeight;"
                      "        for (let i = 0; i < numCircles; i++) {"
                      "            const radius = Math.random() * (maxRadius - minRadius) + minRadius;"
                      "            const x = Math.random() * (canvas.width - 2 * radius) + radius;"
                      "            const y = Math.random() * (canvas.height - 2 * radius) + radius;"
                      "            const dx = (Math.random() - 0.5) * maxSpeed * 2;"
                      "            const dy = (Math.random() - 0.5) * maxSpeed * 2;"
                      "            const color = colors[Math.floor(Math.random() * colors.length)];"
                      "            circles.push(new Circle(x, y, radius, dx, dy, color));"
                      "        }"
                      "    }"
                      "    function animate() {"
                      "        requestAnimationFrame(animate);"
                      "        ctx.clearRect(0, 0, canvas.width, canvas.height);"
                      "        circles.forEach(circle => {"
                      "            circle.update();"
                      "        });"
                      "    }"
                      "    init();"
                      "    animate();"
                      "});"
                      "</script>"
                      "</head><body>"
                      "<canvas id='canvas'></canvas>"
                      "</body></html>\r\n";

    send(client_socket, response, strlen(response), 0);
}

int main() {
    int server_fd, client_fds[MAX_CLIENTS], max_clients = 0;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client_fds[i] = 0;
    }

    while (1) {
        FD_ZERO(&readfds); // Clear set
        FD_SET(server_fd, &readfds); // Add server socket to set

        // Add all active client sockets to set
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (client_fds[i] > 0) {
                FD_SET(client_fds[i], &readfds);
            }
        }

        // Find the highest file descriptor
        int max_fd = server_fd;
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (client_fds[i] > max_fd) {
                max_fd = client_fds[i];
            }
        }

        // Wait for activity on sockets using select()
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        // If server socket is ready, there is a new connection
        if (FD_ISSET(server_fd, &readfds)) {
            int new_client_fd;

            if ((new_client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, IP is : %s, port : %d\n", new_client_fd, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            // Add new socket to array of sockets
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (client_fds[i] == 0) {
                    client_fds[i] = new_client_fd;
                    if (i >= max_clients) {
                        max_clients = i + 1;
                    }
                    break;
                }
            }
        }

        // Check all client sockets for activity
        for (int i = 0; i < max_clients; ++i) {
            int client_socket = client_fds[i];
            if (FD_ISSET(client_socket, &readfds)) {
                int valread;
                if ((valread = recv(client_socket, buffer, BUFFER_SIZE, 0)) == 0) {
                    printf("Client disconnected, socket fd is %d\n", client_socket);
                    close(client_socket);
                    client_fds[i] = 0;
                } else {
                    if (strncmp(buffer, "GET", 3) == 0) {
                        // Send HTTP response with animated circles
                        send_html_response(client_socket);
                        close(client_socket);
                        client_fds[i] = 0;
                    } else {
                        // Echo back to client
                        buffer[valread] = '\0';
                        printf("\nMessage: %sFrom socket_fd: %d\nTime: %s\n", buffer, client_socket, get_current_time());
                        send(client_socket, buffer, valread, 0);
                    }
                }
            }
        }
    }

    return 0;
}
