#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serv_addr = {0};
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	
	connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	char buffer[1024] = {0};
	read(sockfd, buffer, sizeof(buffer));
	printf("Recieved: %s", buffer);

	close(sockfd);
	return 0;
}
