#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX 1024
#define TOTAL_FRAMES 10

int main() {
    srand(time(NULL));
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    char buffer[MAX];
    int expected = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);
    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, (socklen_t*)&len);
    printf("Receiver: Connected to sender\n");

    while (expected < TOTAL_FRAMES) {
        int n = recv(connfd, buffer, sizeof(buffer), 0);
        if (n <= 0) continue;

        buffer[n] = '\0';

        int frame_no;
        sscanf(buffer, "Frame %d", &frame_no);

        if (rand() % 6 == 0) {
            printf("Receiver: Simulating ACK LOSS for frame %d\n", frame_no);
            continue;
        }

        if (frame_no == expected) {
            printf("Receiver: Received %s in-order\n", buffer);
            char ackmsg[20];
            sprintf(ackmsg, "%d", frame_no);
            send(connfd, ackmsg, strlen(ackmsg), 0);
            expected++;
        } else {
            printf("Receiver: Discarded %s (expected %d)\n", buffer, expected);
            char ackmsg[20];
            sprintf(ackmsg, "%d", expected - 1);
            send(connfd, ackmsg, strlen(ackmsg), 0);
        }
    }

    printf("Receiver: All frames received\n");
    close(connfd);
    close(sockfd);
    return 0;
}