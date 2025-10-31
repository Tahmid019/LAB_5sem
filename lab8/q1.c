#include "help.c"

int main(){
    int server, newsoc;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFF];
    char *hello = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><head><title>Web Server</title></head>"
        "<body><h1>Welcome</h1>"
        "<p>Testing Page.</p>"
        "</body></html>";
    
    if((server = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Sock Failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("Bind Failed");
        close(server);
        exit(1);
    }

    if(listen(server, 5) < 0){
        perror("Listen failed");
        close(server);
        exit(1);
    }

    printf("Server running\n");

    while(1){
        printf("Waiting ... \n");

        if((newsoc = accept(server, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
            perror("Accept Failed\n");
            continue;
        }

        memset(buffer, 0, BUFF);
        read(newsoc, buffer, BUFF-1);
        printf("Recv requ: %s\n", buffer);

        write(newsoc, hello, strlen(hello));

        close(newsoc);
    }

    close(server);

    return 0;
}