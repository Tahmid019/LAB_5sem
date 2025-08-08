#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Connected to server.\n");

    while(1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(sockfd, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        int val = recv(sockfd, buffer, sizeof(buffer), 0);
        if (val <= 0) break;

        printf("Server: %s", buffer);
    }

    close(sockfd);
    return 0;
}
