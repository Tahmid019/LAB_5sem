#include "help.c"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct Student s;

    printf("Enter Roll:");
    scanf("%d", &s.roll);
    getchar();

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter Dept: ");
    fgets(s.dept, sizeof(s.dept), stdin);
    s.dept[strcspn(s.dept, "\n")] = 0;

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket Creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        perror("Invalid addr");
        return -1;
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Connection Failed");
        return -1;
    }

    send(sock, &s, sizeof(s), 0);
    printf("Student detailed sent");

    close(sock);
    return 0;
}