#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX 1024
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10
#define TIMEOUT 3   

int sockfd;
struct sockaddr_in servaddr;

void send_frame(int frame_no) {
    char buffer[MAX];
    sprintf(buffer, "Frame %d", frame_no);

    if (rand() % 5 == 0) {
        printf("Sender: Simulating DROP of %s\n", buffer);
        return; 
    }

    send(sockfd, buffer, strlen(buffer), 0);
    printf("Sender: Sent %s\n", buffer);
}

int main() {
    srand(time(NULL));
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("Sender: Connected to receiver\n");

    int base = 0, nextseq = 0;
    char ackbuf[MAX];

    while (base < TOTAL_FRAMES) {
        while (nextseq < base + WINDOW_SIZE && nextseq < TOTAL_FRAMES) {
            send_frame(nextseq);
            nextseq++;
        }

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        struct timeval tv;
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        int rv = select(sockfd + 1, &fds, NULL, NULL, &tv);

        if (rv == 0) {
            printf("Sender: Timeout! Resending from frame %d\n", base);
            for (int i = base; i < nextseq; i++) {
                send_frame(i);
            }
        } else {
            int n = recv(sockfd, ackbuf, sizeof(ackbuf), 0);
            if (n > 0) {
                ackbuf[n] = '\0';
                int ack_no = atoi(ackbuf);
                printf("Sender: Received ACK for %d\n", ack_no);
                base = ack_no + 1;
            }
        }
    }

    printf("Sender: All frames sent successfully\n");
    close(sockfd);
    return 0;
}