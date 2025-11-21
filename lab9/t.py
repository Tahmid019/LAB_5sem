import cv2, socket, struct, threading, time

def sendall(sock, data):
    while data:
        sent = sock.send(data)
        if sent <= 0: raise ConnectionError
        data = data[sent:]

class Clients:
    def __init__(self): self.cs, self.l = [], threading.Lock()
    def add(self, c, a):
        with self.l: print("[+] Connected", a); c.setsockopt(6,1,1); self.cs.append((c,a))
    def send(self, data):
        dead=[]; hdr=struct.pack(">I", len(data))
        with self.l:
            for c,a in self.cs:
                try: sendall(c,hdr); sendall(c,data)
                except: print("[!] Dead", a); dead.append(c)
            self.cs=[x for x in self.cs if x[0] not in dead]
            for d in dead: d.close()

def accept(s, mgr):
    while True: mgr.add(*s.accept())

def stream(src=0, w=640, h=480, fps=30, q=80):
    cap=cv2.VideoCapture(int(src) if str(src).isdigit() else src)
    cap.set(3,w); cap.set(4,h)

    srv=socket.socket(); srv.setsockopt(socket.SOL_SOCKET,2,1)
    srv.bind(("0.0.0.0",5000)); srv.listen()
    print("[*] Server running on 5000")

    mgr=Clients()
    threading.Thread(target=accept, args=(srv,mgr), daemon=True).start()

    enc=[1,q]; dt=1/fps; t=time.perf_counter()
    while True:
        ok,f=cap.read()
        if not ok: cap.set(1,0); continue
        ok,j=cv2.imencode(".jpg", cv2.resize(f,(w,h)), enc)
        if ok: mgr.send(j.tobytes())
        t+=dt; d=t-time.perf_counter()
        time.sleep(d if d>0 else 0); t=time.perf_counter()

if __name__=="__main__":
    stream()
