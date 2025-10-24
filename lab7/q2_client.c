#include "help.c"

#define SRV "127.0.0.1"
#define MAX_PAY 64
#define TOTAL_FRAMES 20
#define WINDOW 4
#define TIMEOUT_MS 700

int send_all(int fd, const void *buf, int len){
    int sent=0;
    while(sent<len){
        int s = send(fd, (char*)buf+sent, len-sent, 0);
        if(s<=0) return s;
        sent+=s;
    }
    return sent;
}
long now_ms(){
    struct timeval tv; gettimeofday(&tv,NULL);
    return tv.tv_sec*1000L + tv.tv_usec/1000;
}

int main(){
    int s=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in sv={.sin_family=AF_INET,.sin_port=htons(PORT)};
    inet_pton(AF_INET, SRV, &sv.sin_addr);
    if(connect(s,(struct sockaddr*)&sv,sizeof(sv))<0){ perror("connect"); return 1; }
    printf("Connected to server %s:%d\n", SRV, PORT);

    int base=0, next=0;
    int acked[TOTAL_FRAMES]; memset(acked,0,sizeof(acked));
    long timers[TOTAL_FRAMES]; memset(timers,0,sizeof(timers));
    char payload[MAX_PAY];

    fd_set rf;
    while(base < TOTAL_FRAMES){
        while(next < base + WINDOW && next < TOTAL_FRAMES){
            // build frame: seq (4), len (4), payload
            int seq = next;
            int len = snprintf(payload, MAX_PAY, "Frame-%02d\n", seq);
            int seq_net = htonl(seq), len_net = htonl(len);
            send_all(s, &seq_net, 4);
            send_all(s, &len_net, 4);
            send_all(s, payload, len);
            timers[seq] = now_ms();
            printf("[sent] frame %d\n", seq);
            next++;
        }

        // ACK wait / timeout
        long earliest_timeout = -1;
        for(int i=base;i<next;i++) if(!acked[i]){
            long t = timers[i] + TIMEOUT_MS - now_ms();
            if(t < 0) { earliest_timeout = 0; break; }
            if(earliest_timeout<0 || t < earliest_timeout) earliest_timeout = t;
        }
        if(earliest_timeout < 0) earliest_timeout = TIMEOUT_MS;

        FD_ZERO(&rf); FD_SET(s, &rf);
        struct timeval tv;
        tv.tv_sec = earliest_timeout/1000;
        tv.tv_usec = (earliest_timeout%1000)*1000;
        int rv = select(s+1, &rf, NULL, NULL, &tv);
        if(rv > 0 && FD_ISSET(s,&rf)){
            int ack_net;
            int r = recv(s, &ack_net, 4, 0);
            if(r<=0) { perror("recv"); break; }
            int ack = ntohl(ack_net);
            if(ack >= 0 && ack < TOTAL_FRAMES && !acked[ack]){
                acked[ack]=1;
                printf("[rcv] ACK %d\n", ack);
                // slide base
                while(base < TOTAL_FRAMES && acked[base]) base++;
            }
        } else {
            // timeout: retransmit 
            printf("[timeout] retransmitting window %d..%d\n", base, next-1);
            for(int i=base;i<next;i++) if(!acked[i]){
                int seq = i;
                int len = snprintf(payload, MAX_PAY, "Frame-%02d\n", seq);
                int seq_net = htonl(seq), len_net = htonl(len);
                send_all(s, &seq_net, 4);
                send_all(s, &len_net, 4);
                send_all(s, payload, len);
                timers[seq] = now_ms();
                printf("[r-sent] frame %d\n", seq);
            }
        }
    }

    printf("All frames ACKed. Done.\n");
    close(s);
    return 0;
}
