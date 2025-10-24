#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {AF_INET, htons(PORT)};
    inet_pton(AF_INET, SERVER_IP, &serv.sin_addr);
    connect(s, (struct sockaddr*)&serv, sizeof(serv));

    FILE *f = fopen("received.jpg", "wb");
    char buf[1024];
    int n, total = 0;

    while ((n = recv(s, buf, sizeof(buf), 0)) > 0) {
        fwrite(buf, 1, n, f);
        total += n;
    }

    printf("Image received (%d bytes).\n", total);
    fclose(f);
    close(s);
    return 0;
}
