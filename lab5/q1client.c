#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3000

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    int num;
    long long result;

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    addr_len = sizeof(server_addr);

    while (1) {
        printf("Enter a number (-1 to exit): ");
        scanf("%d", &num);
        if (num < 0) break;

        sendto(client_socket, &num, sizeof(num), 0,
               (struct sockaddr*)&server_addr, addr_len);

        recvfrom(client_socket, &result, sizeof(result), 0,
                 (struct sockaddr*)&server_addr, &addr_len);

        printf("Factorial of %d is %lld\n", num, result);
    }

    close(client_socket);
    return 0;
}
