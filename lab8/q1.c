#include "help.c"

int main(){
    int server, newsoc;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFF];


    FILE *fp = fopen("index.html", "r");
    if(!fp){
        perror("cannot open index.html");
        close(newsoc);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    char *filebuf = malloc(fsize+1);
    fread(filebuf, 1, fsize, fp);
    filebuf[fsize] = '\0';
    fclose(fp);

    char header[256];
    snprintf(header, sizeof(header), 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        fsize
    );
    
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

        write(newsoc, header, strlen(header));
        write(newsoc, filebuf, fsize);

        free(filebuf);
        close(newsoc);
    }

    close(server);

    return 0;
}