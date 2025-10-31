#include "help.c"

int main(){
    int server, newsock;
    struct sockaddr_in address;
    struct Student s;
    socklen_t addrlen = sizeof(address);

    if((server = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket Failed");
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

    if(listen(server, 3) < 0){
        perror("Listen Err");
        close(server);
        exit(1);
    }

    printf("Server Listening ... ");

    if((newsock = accept(server, (struct sockaddr*)&address, &addrlen)) < 0){
        perror("Accept");
        close(server);
        exit(1);
    }

    size_t bytes_recv = recv(newsock, &s, sizeof(s), 0);
    if(bytes_recv < 0){
        perror("Recv Failed");
    }else{
        printf("\nReceived Student Details:\n");
        printf("Roll No: %d\n", s.roll);
        printf("Name: %s\n", s.name);
        printf("Department: %s\n", s.dept);
        printf("Marks: %.2f\n", s.marks);
    }

    close(newsock);
    close(server);
    return 0;
}
