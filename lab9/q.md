# Lab 9
10.  Write a socket program to implement real-time video streaming. Explanation: The client requests video live streaming to the server and the server sends the video live streaming to the client.

```
sudo apt install libopencv-dev
```

```
g++ client.cpp -o client $(pkg-config --cflags --libs opencv4)
g++ server.cpp -o server $(pkg-config --cflags --libs opencv4)
```