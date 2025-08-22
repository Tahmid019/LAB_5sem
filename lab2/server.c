#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define BUF_SIZE 1024
#define MAX_CLIENTS 10
#define NAME_LEN 50

int main() {
    int server_fd, new_socket, client_sockets[MAX_CLIENTS];
    char client_names[MAX_CLIENTS][NAME_LEN];
    struct sockaddr_in address;
    char buffer[BUF_SIZE], msg[BUF_SIZE + NAME_LEN];
    fd_set readfds;
    int max_sd, activity, valread, sd;
    socklen_t addrlen = sizeof(address);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
        memset(client_names[i], 0, NAME_LEN);
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Chat server started on port %d\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0) {
                perror("accept");
                exit(1);
            }

            printf("New connection from %s:%d\n",
                   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;

                    memset(buffer, 0, BUF_SIZE);
                    int val = recv(new_socket, buffer, BUF_SIZE, 0);
                    if (val > 0) {
                        buffer[strcspn(buffer, "\n")] = 0;
                        strncpy(client_names[i], buffer, NAME_LEN - 1);
                        printf("Client %d registered as '%s'\n", i + 1, client_names[i]);
                    } else {
                        close(new_socket);
                        client_sockets[i] = 0;
                    }
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (FD_ISSET(sd, &readfds)) {
                valread = recv(sd, buffer, BUF_SIZE, 0);
                if (valread <= 0) {
                    getpeername(sd, (struct sockaddr *)&address, &addrlen);
                    printf("Client '%s' disconnected (%s:%d)\n",
                           client_names[i],
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    close(sd);
                    client_sockets[i] = 0;
                    memset(client_names[i], 0, NAME_LEN);
                } else {
                    buffer[valread] = '\0';
                    snprintf(msg, sizeof(msg), "%s: %s", client_names[i], buffer);
                    printf("%s", msg);

                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (client_sockets[j] != 0 && client_sockets[j] != sd) {
                            send(client_sockets[j], msg, strlen(msg), 0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
