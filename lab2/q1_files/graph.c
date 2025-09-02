#include "graph.h"
#include <string.h> // For memset

// Define global variables
Node* adjList[MAX_PROCESSES];
int adjMatrix[MAX_PROCESSES][MAX_PROCESSES];
int pids[MAX_PROCESSES];
int pidCount = 0;

void init_graph() {
    pidCount = 0;
    // Initialize matrix with 0s
    memset(adjMatrix, 0, sizeof(adjMatrix));
    // Initialize list heads to NULL
    for (int i = 0; i < MAX_PROCESSES; i++) {
        adjList[i] = NULL;
    }
}

int get_index(int pid) {
    for (int i = 0; i < pidCount; i++) {
        if (pids[i] == pid) {
            return i;
        }
    }

    if (pidCount < MAX_PROCESSES) {
        pids[pidCount] = pid;
        return pidCount++; // Return the new index and then increment
    }

    fprintf(stderr, "Error: Maximum number of processes exceeded.\n");
    exit(EXIT_FAILURE);
}

void add_edge_list(int parent_pid, int child_pid) {
    int p_index = get_index(parent_pid);
    
    // Child PID does not need an index for the list, just its value
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    newNode->pid = child_pid;
    newNode->next = adjList[p_index];
    adjList[p_index] = newNode;
}

void add_edge_matrix(int parent_pid, int child_pid) {
    int p_index = get_index(parent_pid);
    int c_index = get_index(child_pid);
    adjMatrix[p_index][c_index] = 1;
}

void print_adjacency_list() {
    printf("\n--- Adjacency List Representation ---\n");
    for (int i = 0; i < pidCount; i++) {
        printf("  Process %d -> ", pids[i]);
        Node* current = adjList[i];
        if (!current) {
            printf("NULL");
        }
        while (current) {
            printf("%d -> ", current->pid);
            current = current->next;
            if (!current) {
                printf("NULL");
            }
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
}

void print_adjacency_matrix() {
    printf("\n--- Adjacency Matrix Representation ---\n");
    printf("        ");
    for (int i = 0; i < pidCount; i++) {
        printf("%-7d", pids[i]);
    }
    printf("\n");

    for (int i = 0; i < pidCount; i++) {
        printf("%-7d ", pids[i]);
        for (int j = 0; j < pidCount; j++) {
            printf("%-7d", adjMatrix[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
}