#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROCESSES 100

typedef struct Node {
    int pid;
    struct Node* next;
} Node;

Node* adjList[MAX_PROCESSES];
int adjMatrix[MAX_PROCESSES][MAX_PROCESSES];
int pids[MAX_PROCESSES];
int pidCount = 0;

int getIndex(int pid) {
    for (int i = 0; i < pidCount; i++) {
        if (pids[i] == pid) return i;
    }
    if (pidCount < MAX_PROCESSES) {
        pids[pidCount] = pid;
        pidCount++;
        return pidCount - 1;
    }
    fprintf(stderr, "Too many processes\n");
    exit(1);
}

void add_list(int parent, int child) {
    int pIndex = getIndex(parent);
    Node* newNode = malloc(sizeof(Node));
    newNode->pid = child;
    newNode->next = adjList[pIndex];
    adjList[pIndex] = newNode;
}

void add_mat(int parent, int child) {
    int pIndex = getIndex(parent);
    int cIndex = getIndex(child);
    adjMatrix[pIndex][cIndex] = 1;
}

void print_list() {
    printf("\nAdjacency List:\n");
    for (int i = 0; i < pidCount; i++) {
        printf("Process %d -> ", pids[i]);
        Node* temp = adjList[i];
        while (temp) {
            printf("%d ", temp->pid);
            temp = temp->next;
        }
        printf("\n");
    }
}

void print_mat() {
    printf("\nAdjacency Matrix:\n   ");
    for (int i = 0; i < pidCount; i++) printf("%5d", pids[i]);
    printf("\n");
    for (int i = 0; i < pidCount; i++) {
        printf("%5d", pids[i]);
        for (int j = 0; j < pidCount; j++)
            printf("%5d", adjMatrix[i][j]);
        printf("\n");
    }
}

int main() {
    int root = getpid();
    getIndex(root);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        add_list(getppid(), getpid());
        add_mat(getppid(), getpid());
        sleep(1);
    } else {
        add_list(getpid(), pid1);
        add_mat(getpid(), pid1);
        pid_t pid2 = fork();
        if (pid2 == 0) {
            add_list(getppid(), getpid());
            add_mat(getppid(), getpid());
            sleep(1);
        } else {
            add_list(getpid(), pid2);
            add_mat(getpid(), pid2);
            sleep(2);
            print_list();
            print_mat();
        }
    }

    execlp("ps", "ps", "-ef", NULL);

    wait(NULL);

    return 0;
}