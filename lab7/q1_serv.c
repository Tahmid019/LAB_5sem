#include "help.c"

const char *wl[] = {"ls", "date", "whoami", "pwd", "cat", "echo", NULL};

int allowed(const char *c){
    char t[128]; strncpy(t, c, sizeof(t)-1); t[sizeof(t) - 1] = 0;
    char *p = strtok(t, "\t\n"); if(!p) return 0;
    for(const char **q=wl; *q; ++q) if(!strcmp(p, *q)) return 1;
    return 0;
}

int main(){
    int s = socket(AF_INET, SOCK_STREAM, 0), c;
    struct sockaddr_in a = {.sin_family=AF_INET, .sin_port=htons(PORT)};
    inet_pton(AF_INET, HOST, &a.sin_addr);
    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 5);

    puts("RCE server on 127.0.0.1:12345\n");

    while((c=accept(s, NULL, NULL)) > 0){
        char cmd[512]= {0};
        ssize_t n = recv(c, cmd, sizeof(cmd)-1, 0);
        
        if (n <= 0) {close(c); continue;}  cmd[n] = 0;
        if(!allowed(cmd)) {
            send(c, "ERRPR: not allowed\n", 19, 0);
            close(c);
            continue;
        }
        printf("command recv: %s", cmd);
        FILE *fp = popen(cmd, "r");
        if(!fp){
            send(c, "ERROR\n", 6, 0);
            close(c);
            continue;
        }

        char buf[1024]; size_t r;
        while((r = fread(buf, 1, sizeof(buf), fp)) > 0) send(c, buf, r, 0);
        pclose(fp); close(c);
    }

    close(s); return 0;
}