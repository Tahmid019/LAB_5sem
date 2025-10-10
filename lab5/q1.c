#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 3000

long long factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("In Server listening on port: %d\n", PORT);
    
    int num;
    client_addr_len = sizeof(client_addr);

    while (1) {
        ssize_t bytes_received = recvfrom(server_socket, &num, sizeof(num), 0, (struct sockaddr*)&client_addr, &client_addr_len);

        if (bytes_received < 0) {
            perror("Receiving error");
            continue; 
        }

        long long result = factorial(num);

        sendto(server_socket, &result, sizeof(result), 0, (struct sockaddr*)&client_addr, client_addr_len);

        printf("Factorial of %d sent to client with IP Address - %s, and PORT Number - %d\n",
               num, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    close(server_socket);
    return 0;
}