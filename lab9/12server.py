import cv2
import socket
import struct
import threading
import time


def sendall(sock, data):
    total = 0
    while total < len(data):
        sent = sock.send(data[total:])
        if sent <= 0:
            raise ConnectionError("send failed")
        total += sent


class ClientManager:
    def __init__(self):
        self.clients = []
        self.lock = threading.Lock()

    def add(self, conn, addr):
        with self.lock:
            print(f"[+] Client connected {addr}")
            conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
            self.clients.append((conn, addr))

    def remove(self, conn):
        with self.lock:
            self.clients = [(c, a) for (c, a) in self.clients if c is not conn]

    def broadcast(self, payload):
        """Send frame to all clients"""
        dead = []
        header = struct.pack(">I", len(payload))

        with self.lock:
            for conn, addr in list(self.clients):
                try:
                    sendall(conn, header)
                    sendall(conn, payload)
                except Exception:
                    print(f"[!] Removing dead client {addr}")
                    dead.append(conn)

        for d in dead:
            try: d.close()
            except: pass
            self.remove(d)


def accept_clients(server_sock, manager: ClientManager):
    while True:
        conn, addr = server_sock.accept()
        manager.add(conn, addr)


def live_stream(video_source="0", width=640, height=480, fps=30, quality=80):
    # Video source
    if video_source.isdigit():
        src = int(video_source)
    else:
        src = video_source

    print(f"[*] Opening source: {src}")
    cap = cv2.VideoCapture(src)

    if not cap.isOpened():
        raise SystemExit(f"[!] Cannot open source: {video_source}")

    cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

    # Socket server
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_sock.bind(("0.0.0.0", 5000))
    server_sock.listen(10)

    print("[*] Live stream server running on port 5000")
    print("[*] Clients can connect anytime.")

    manager = ClientManager()
    threading.Thread(target=accept_clients, args=(server_sock, manager), daemon=True).start()

    encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), quality]
    frame_interval = 1.0 / fps
    next_time = time.perf_counter()

    while True:
        ret, frame = cap.read()
        if not ret:
            print("[!] Video ended or frame error. Resetting...")
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
            continue

        frame = cv2.resize(frame, (width, height))
        ok, jpg = cv2.imencode(".jpg", frame, encode_param)
        if not ok:
            continue

        manager.broadcast(jpg.tobytes())

        # FPS control
        next_time += frame_interval
        delay = next_time - time.perf_counter()
        if delay > 0:
            time.sleep(delay)
        else:
            next_time = time.perf_counter()


if __name__ == "__main__":

    live_stream(video_source="0")
