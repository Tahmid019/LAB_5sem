#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define BUF_SIZE 1024

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE], msg[BUF_SIZE];
    fd_set readfds;

    if (argc < 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server %s:%d\n", argv[1], PORT);

    printf("Enter your name: ");
    char name[50];
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  

    send(sockfd, name, strlen(name), 0);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(sockfd, &readfds);

        int maxfd = sockfd;
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(0, &readfds)) {
            memset(buffer, 0, BUF_SIZE);
            if (fgets(buffer, BUF_SIZE, stdin) == NULL) break;

            snprintf(msg, sizeof(msg), "%s: %s", name, buffer);
            send(sockfd, msg, strlen(msg), 0);
        }

        if (FD_ISSET(sockfd, &readfds)) {
            memset(buffer, 0, BUF_SIZE);
            int val = recv(sockfd, buffer, BUF_SIZE, 0);
            if (val <= 0) {
                printf("Server closed connection.\n");
                break;
            }
            printf("%s", buffer);  
        }
    }

    close(sockfd);
    return 0;
}
