#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);

    std::cout << "Windows MJPEG Stream Server: http://localhost:8080/stream\n";

    while (true) {
        sockaddr_in client;
        int len = sizeof(client);
        SOCKET client_fd = accept(server_fd, (sockaddr*)&client, &len);

        char buf[4096] = {0};
        recv(client_fd, buf, sizeof(buf)-1, 0);

        std::string req(buf);
        std::cout << req << std::endl;

        if (req.find("GET / ") != std::string::npos || req.find("GET /HTTP") != std::string::npos) {
            std::string html =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n\r\n"
                "<html><body><h1>Live Stream</h1>"
                "<img src=\"/stream\" width=640 height=480>"
                "</body></html>";
            send(client_fd, html.c_str(), html.size(), 0);
            closesocket(client_fd);
            continue;
        }

        if (req.find("GET /stream") != std::string::npos) {
            std::string header =
                "HTTP/1.1 200 OK\r\n"
                "Cache-Control: no-cache\r\n"
                "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";

            send(client_fd, header.c_str(), header.size(), 0);

            // ✅ OPEN WINDOWS CAMERA USING MEDIA FOUNDATION (MSMF)
            cv::VideoCapture cam(0, cv::CAP_MSMF);
            if (!cam.isOpened()) {
                std::string err = "--frame\r\nCamera open failed\r\n";
                send(client_fd, err.c_str(), err.size(), 0);
                closesocket(client_fd);
                continue;
            }

            cam.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            cam.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

            while (true) {
                cv::Mat frame;
                cam >> frame;
                if (frame.empty()) break;

                std::vector<uchar> jpg;
                cv::imencode(".jpg", frame, jpg);

                std::string part =
                    "--frame\r\n"
                    "Content-Type: image/jpeg\r\n"
                    "Content-Length: " + std::to_string(jpg.size()) + "\r\n\r\n";

                send(client_fd, part.c_str(), part.size(), 0);
                send(client_fd, (char*)jpg.data(), jpg.size(), 0);
                send(client_fd, "\r\n", 2, 0);

                Sleep(30);
            }
        }

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
