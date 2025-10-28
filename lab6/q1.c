#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

atomic_int *mutex;

atomic_int init_mut(atomic_int* mutex, int a){
    *mutex = a;
    return *mutex;
}

atomic_int mut_wait(atomic_int *mut){
    while(*mut <= 0);
    --*mut;
    return *mut;
}

atomic_int mut_signal(atomic_int *mut){
    ++*mut;
    return  *mut;
}

void* process(void* arg) {
    int id = *((int*)arg);

    mut_wait(&mutex);

    printf("CS Enter id: %d \n", id);
    sleep(1);
    printf("CS Eject id: %d \n", id);

    mut_signal(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    init_mut(&mutex, 1);
    
    pthread_create(&t1, NULL, process, &id1);
    pthread_create(&t2, NULL, process, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
