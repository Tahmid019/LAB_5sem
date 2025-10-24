#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define IMG_NAME "image.jpg"

int main() {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(PORT), INADDR_ANY};
    bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(sfd, 1);
    printf("Server ready. Waiting for client...\n");

    int cfd = accept(sfd, NULL, NULL);
    FILE *f = fopen(IMG_NAME, "rb");
    if (!f) { perror("File open failed"); return 1; }

    char buf[1024];
    int n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
        send(cfd, buf, n, 0);

    printf("Image sent successfully.\n");
    fclose(f);
    close(cfd);
    close(sfd);
    return 0;
}
