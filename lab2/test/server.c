#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uinstd.h> 
#include <netinet/in.h>

#define PORT 8080
#define BUFF_SIZE 1024
#define MAX_CLIENTS 10
#define NAME_LEN 1024

int main(){
    int server_fd, client_fd[MAX_CLIENTS];
    char client_names[MAX_CLIENTS][NAME_LEN];
    struct sockaddr_in address;
    char buffer[BUFF_SIZE];
    size_t addrLen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("[!] Err in Bind")
        exit(1);
    }
    if(listen(server_fd, 10) < 0){
        perror("[!] Err in Listen");
        exit(1);
    }

    printf("[+] Server Open || Port %d\n", PORT);

    for(int i = 0; i < MAX_CLIENTS; i++){
        client_fd[i] = 0;
        memset(client_names[i], 0, NAME_LEN);
    }

    while(1){

    }

}