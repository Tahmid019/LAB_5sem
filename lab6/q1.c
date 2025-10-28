#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex; 

void* process(void* arg) {
    int id = *((int*)arg);

    printf("Process %d waiting to enter critical section...\n", id);
    sem_wait(&mutex); // wait (lock)
    
    printf("Process %d entered critical section.\n", id);
    sleep(2); // simulate work
    printf("Process %d leaving critical section.\n", id);
    
    sem_post(&mutex); // signal (unlock)
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    sem_init(&mutex, 0, 1); // init bin-sema 1

    pthread_create(&t1, NULL, process, &id1);
    pthread_create(&t2, NULL, process, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&mutex);
    return 0;
}
