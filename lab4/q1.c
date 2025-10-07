#include "help.h"

#define N 2
bool flag[N] = {false};
int turn = 0;

void *process(void *arg){
    int id = *((int*)arg);
    int other = 1-id;
    while(true){
        flag[id] = true;
        turn = other;
        while(flag[other] && turn == other);
        printf("[=] Process %d || CS\n", id);
        flag[id] = false;
        printf("[-] Process %d || RS\n", id);
    }
    return NULL;
}

 void solve() {
    pthread_t t1, t2;
    int id1 = 0, id2 = 1;
    pthread_create(&t1, NULL, process, &id1);
    pthread_create(&t2, NULL, process, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
 }

int main(){
    EXE_TIME(solve, "Solve: ");
    return 0;
}