#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROCESSES 100
#define LEVELS 4

typedef struct Node {
    int pid;
    struct Node* next;
} Node;

Node* adjList[MAX_PROCESSES] = {NULL};
int adjMatrix[MAX_PROCESSES][MAX_PROCESSES] = {0};
int pids[MAX_PROCESSES];
int pidCount = 0;

int getIndex(int pid) {
    for (int i = 0; i < pidCount; i++) {
        if (pids[i] == pid) return i;
    }
    if (pidCount < MAX_PROCESSES) {
        pids[pidCount] = pid;
        return pidCount++;
    }
    fprintf(stderr, "Error: Too many processes.\n");
    exit(1);
}

void add_list(int parent, int child) {
    int pIndex = getIndex(parent);
    getIndex(child);
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        perror("malloc");
        exit(1);
    }
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
    printf("\npid: %d\n", getpid());
    for (int i = 0; i < pidCount; i++) {
        printf("Process %-6d -> ", pids[i]);
        Node* temp = adjList[i];
        while (temp) {
            printf("%d ", temp->pid);
            temp = temp->next;
        }
        printf("\n");
    }
}

void print_mat() {
    printf("\npid: %d\n", getpid());
    printf("        ");
    for (int i = 0; i < pidCount; i++) printf("%6d", pids[i]);
    printf("\n");
    for (int i = 0; i < pidCount; i++) {
        printf("Process %-6d", pids[i]);
        for (int j = 0; j < pidCount; j++)
            printf("%6d", adjMatrix[i][j]);
        printf("\n");
    }
}

int main() {
    getIndex(getpid());
   

    for (int i = 0; i < LEVELS; i++) {
        pid_t child_pid = fork();

        if (child_pid > 0) {
            wait(NULL);
            break;
        } else if (child_pid == 0) {
            add_mat(getppid(), getpid());
            add_list(getppid(), getpid());
            print_list();
            print_mat();
        } else {
            perror("fork");
            exit(1);
        }
    }

    execlp("ps", "ps", "-ef", NULL);

    wait(NULL);
    
    return 0;
}