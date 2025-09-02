#include "help.h"

int main() {
    pid_t lc_pid, rc_pid;
    printf("[=] Main: %d\n", getpid());

    lc_pid = fork();
    if(lc_pid < 0){
        perror("fork failed");
        exit(1);
    }
    if(lc_pid == 0){
        
        EXE_TIME(process_task, "Left Child", "Left Child", 1);
        exit(0);
    }

    rc_pid = fork();
    if(rc_pid < 0){
        perror("fork failed");
        exit(1);
    }
    if(rc_pid == 0){
        EXE_TIME(process_task, "Right Child", "Right Child", 1);
        exit(0);
    }

    EXE_TIME(process_task, "Parent","Parent", 1);

    wait(NULL);
    wait(NULL);

    return 0;
}