#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define FILE_NAME "file.txt"
#define MAX_FILE_SIZE 4096

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("Socket creation failed");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        error("Setsockopt failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error("Bind failed");
    }

    if (listen(server_fd, 3) < 0) {
        error("Listen failed");
    }

    printf("FTP Server running on port %d. Waiting for connection...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        error("Accept failed");
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    FILE *file = NULL;
    char file_buffer[MAX_FILE_SIZE] = {0};
    long file_size = 0;
    size_t bytes_read = 0;

    file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        perror("Error opening file.txt");
        const char *fail_msg = "ERROR: File not found on server.\n";
        send(new_socket, fail_msg, strlen(fail_msg), 0);
    } else {
        printf("Attempting to send file: %s\n", FILE_NAME);

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (file_size > MAX_FILE_SIZE) {
            fprintf(stderr, "File is too large (%ld bytes). Only sending first %d bytes.\n", file_size, MAX_FILE_SIZE);
        }

        bytes_read = fread(file_buffer, 1, (file_size > MAX_FILE_SIZE ? MAX_FILE_SIZE : file_size), file);
        fclose(file);

        if (send(new_socket, file_buffer, bytes_read, 0) == -1) {
            perror("Send failed");
        } else {
            printf("Successfully sent %zu bytes of data.\n", bytes_read);
        }
    }

    close(new_socket);
    close(server_fd);
    printf("Connection closed. Server shutting down.\n");

    return 0;
}
