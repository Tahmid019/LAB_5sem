#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PROCESSES 100

// ----- Data Structures -----

// Node for Adjacency List
typedef struct Node {
    int pid;
    struct Node* next;
} Node;

// ----- Global Variables (declared as extern) -----

extern Node* adjList[MAX_PROCESSES];
extern int adjMatrix[MAX_PROCESSES][MAX_PROCESSES];
extern int pids[MAX_PROCESSES];
extern int pidCount;

// ----- Function Prototypes -----

/**
 * @brief Initializes graph data structures.
 */
void init_graph();

/**
 * @brief Gets the internal array index for a given PID.
 * If the PID is new, it's added to the list.
 * @param pid The process ID.
 * @return The integer index for the PID.
 */
int get_index(int pid);

/**
 * @brief Adds a directed edge from parent to child in the adjacency list.
 * @param parent_pid The PID of the parent process.
 * @param child_pid The PID of the child process.
 */
void add_edge_list(int parent_pid, int child_pid);

/**
 * @brief Adds a directed edge from parent to child in the adjacency matrix.
 * @param parent_pid The PID of the parent process.
 * @param child_pid The PID of the child process.
 */
void add_edge_matrix(int parent_pid, int child_pid);

/**
 * @brief Prints the entire adjacency list representation of the graph.
 */
void print_adjacency_list();

/**
 * @brief Prints the entire adjacency matrix representation of the graph.
 */
void print_adjacency_matrix();

#endif // GRAPH_H