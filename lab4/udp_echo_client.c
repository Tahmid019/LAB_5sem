// udp_echo_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define SERVER_IP "0.0.0.0" 
#define PORT 8001
#define BUFFER_SIZE 1024

double get_time_in_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    for (int i = 0; i < 5; i++) {
        char message[BUFFER_SIZE];
        sprintf(message, "Hello %d", i);

        double start = get_time_in_ms();

        sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr *)&server_addr, addr_len);

        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                         (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0';

        double end = get_time_in_ms();
        double rtt = end - start;

        printf("Received: %s | RTT = %.2f ms\n", buffer, rtt);
    }

    close(sockfd);
    return 0;
}