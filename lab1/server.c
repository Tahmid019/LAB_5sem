#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr = {0};
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8000);

	bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(sockfd, 1);

	int client_fd = accept(sockfd, NULL, NULL);
	char msg[] = "hello from server\n";

	write(client_fd, msg, sizeof(msg));

	close(client_fd);
	close(sockfd);

	return 0;
}
