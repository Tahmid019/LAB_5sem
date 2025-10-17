#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Socket creation error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address/Address not supported\n");
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection failed");
    }

    printf("Connected to server. Requesting file: file.txt\n");


    printf("\n--- RECEIVED FILE CONTENT ---\n");
    int total_bytes_received = 0;

    while ((valread = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[valread] = '\0';

        printf("%s", buffer);

        total_bytes_received += valread;
    }

    if (valread == 0) {
        printf("\n\n--- END OF FILE (Received %d bytes) ---\n", total_bytes_received);
    } else if (valread == -1) {
        perror("Receive failed");
    }

    close(sock);

    return 0;
}
