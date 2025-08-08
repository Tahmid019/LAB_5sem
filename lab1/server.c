#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[1024];
    size_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);

    printf("Waiting for client...\n");
    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int val = recv(client_fd, buffer, sizeof(buffer), 0);
        if (val <= 0) break;

        printf("Client: %s", buffer);

        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
