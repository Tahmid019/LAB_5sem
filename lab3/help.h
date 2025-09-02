#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#define DELAY 0

#define EXE_TIME(func, name, ...)         \
    do {                            \
        clock_t start = clock();    \
        func(__VA_ARGS__);          \
        clock_t end = clock();      \
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC; \
        printf(" :-: %s %s = \t %f secs \n", #func, name, elapsed); \
    } while(0)


void printInfo(char* name, int is_child, int pid, int ppid, int i){
    if(is_child){
        printf("[%d] %s\t || child: %d || parent: %d",i+1, name, pid, ppid);
    }else{
        printf("[%d] %s\t || parent: %d",i+1, name, pid);
    }
}
void process_task(char* name, int is_child){
    pid_t pid = getpid();
    pid_t ppid = getppid();
    for(int i = 0; i < 10; i++){
        EXE_TIME(printInfo, name, name, is_child, pid, ppid, i);
    }
    usleep(DELAY);
}