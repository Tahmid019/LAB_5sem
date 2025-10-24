#include "help.c"

#define MAX_PAY 256
#define LOSS_PROB 30  

int recv_all(int fd, void *buf, int len){
    int got=0;
    while(got<len){
        int r=recv(fd,(char*)buf+got,len-got,0);
        if(r<=0) return r;
        got+=r;
    }
    return got;
}

int main(){
    srand(time(NULL));
    int s=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in a={.sin_family=AF_INET,.sin_port=htons(PORT),.sin_addr.s_addr=INADDR_ANY};
    bind(s,(struct sockaddr*)&a,sizeof(a));
    listen(s,1);
    printf("SR server listening on %d\n",PORT);
    int c=accept(s,NULL,NULL);
    if(c<0) { perror("accept"); return 1; }

    int base_expected = 0;
    const int MAX_SEQ = 256;
    char buffer[MAX_PAY+8];

    char store[256][MAX_PAY];
    int have[256]={0}, lens[256]={0};

    while(1){
        int seq_net;
        int len_net;
        int r = recv_all(c, &seq_net, 4);
        if(r<=0) break;
        recv_all(c, &len_net, 4);
        int seq = ntohl(seq_net);
        int len = ntohl(len_net);
        if(len>0) recv_all(c, buffer, len);


        // --loss--
        if((rand()%100) < LOSS_PROB){
            printf("[drop] frame %d\n", seq);
            continue;
        }
        printf("[recv] frame %d len=%d\n", seq, len);

        // send ack
        int ack_net = htonl(seq);
        send(c, &ack_net, 4, 0);
        printf("[sent] ACK %d\n", seq);

        // store deliver
        if(seq == base_expected){
            // deliver and advance
            if(len>0) write(1, buffer, len); 
            base_expected = (base_expected + 1) % MAX_SEQ;
       
            while(have[base_expected]){
                write(1, store[base_expected], lens[base_expected]);
                have[base_expected]=0;
                base_expected=(base_expected+1)%MAX_SEQ;
            }
        } else {
            memcpy(store[seq], buffer, len);
            lens[seq]=len;
            have[seq]=1;
        }
    }

    close(c); close(s);
    return 0;
}
