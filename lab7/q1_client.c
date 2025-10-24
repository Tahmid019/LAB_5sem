#include "help.c"

int main(int argc, char **argv){
    char cmd[512] = {0};
    if(argc > 1){
        size_t p = 0;
        for(int i = 1; i < argc; ++i) p += snprintf(cmd+p, sizeof(cmd)-p, "%s%s", i == 1 ? "":" ", argv[i]);
    }else{
        if(!fgets(cmd, sizeof(cmd), stdin)) return 0;
    }

    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sv = {AF_INET, htons(PORT)};
    inet_pton(AF_INET, HOST, &sv.sin_addr);
    if(connect(s, (struct sockaddr*)&sv, sizeof(sv)) < 0){
        perror("connect");
        return 1;
    }
    if(cmd[strlen(cmd)-1]!= '\n') strncat(cmd, "\n", sizeof(cmd) - strlen(cmd)-1);
    send(s, cmd, strlen(cmd), 0);
    shutdown(s, SHUT_WR);
    char buf[1024]; ssize_t n; int tot = 0;
    while((n = recv(s, buf, sizeof(buf), 0))>0){
        fwrite(buf, 1, n, stdout);
        tot += n;
    }
    printf("\n------%d bytes------\n", tot);
    close(s);
    return 0;
}