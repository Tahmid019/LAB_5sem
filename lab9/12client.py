import cv2
import socket
import struct
import numpy as np


def recvall(sock, size):
    data = b""
    while len(data) < size:
        part = sock.recv(size - len(data))
        if not part:
            return None
        data += part
    return data


def main():
    HOST = "127.0.0.1"
    PORT = 5000

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    print("[+] Connected to live stream")

    while True:
        header = recvall(sock, 4)
        if not header:
            break

        size = struct.unpack(">I", header)[0]
        frame_data = recvall(sock, size)
        if frame_data is None:
            break

        img = np.frombuffer(frame_data, dtype=np.uint8)
        frame = cv2.imdecode(img, cv2.IMREAD_COLOR)

        cv2.imshow("Live Stream", frame)
        if cv2.waitKey(1) == ord('q'):
            break

    sock.close()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
