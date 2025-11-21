#include <iostream>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

#define PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::cout << "Connected! Receiving video..." << std::endl;

    while (true) {
        int size = 0;

        // Read frame size first
        if (recv(sock, &size, sizeof(size), MSG_WAITALL) <= 0)
            break;

        std::vector<uchar> buffer(size);
        if (recv(sock, buffer.data(), size, MSG_WAITALL) <= 0)
            break;

        // Decode JPEG buffer into cv::Mat
        cv::Mat img = cv::imdecode(buffer, cv::IMREAD_COLOR);

        if (!img.empty()) {
            cv::imshow("Live Stream", img);
            if (cv::waitKey(1) == 27) break; // ESC to exit
        }
    }

    close(sock);
    return 0;
}
