#include <sys/wait.h>
#include "graph.h"

int main() {
    init_graph();
    
    int root_pid = getpid();
    printf("Root process started with PID: %d\n", root_pid);
    
    // Ensure root PID is in our mapping
    get_index(root_pid);

    // Create first child
    pid_t child1_pid = fork();
    if (child1_pid < 0) {
        perror("fork 1 failed");
        exit(EXIT_FAILURE);
    }

    if (child1_pid == 0) {
        // --- Child 1 Code ---
        // This child does nothing but exit
        exit(0);
    } 
    
    // --- Parent Code ---
    printf("Parent (%d) created child 1 with PID: %d\n", root_pid, child1_pid);
    add_edge_list(root_pid, child1_pid);
    add_edge_matrix(root_pid, child1_pid);
    
    // Create second child
    pid_t child2_pid = fork();
    if (child2_pid < 0) {
        perror("fork 2 failed");
        exit(EXIT_FAILURE);
    }

    if (child2_pid == 0) {
        // --- Child 2 Code ---
        // This child does nothing but exit
        exit(0);
    }

    // --- Parent Code ---
    printf("Parent (%d) created child 2 with PID: %d\n", root_pid, child2_pid);
    add_edge_list(root_pid, child2_pid);
    add_edge_matrix(root_pid, child2_pid);

    // Wait for both children to terminate to prevent zombies
    wait(NULL);
    wait(NULL);

    printf("\nBoth children have terminated. Parent will now print the graph.\n");

    // Print the final representations
    print_adjacency_list();
    print_adjacency_matrix();

    printf("\nParent process finished.\n");

    return 0;
}


//==============================================

// #include <sys/wait.h>
// #include "graph.h"

// #define LEVELS 4

// int main() {
//     init_graph();
    
//     // The main process starts the chain. Register it.
//     printf("Initial process PID: %d\n", getpid());
//     get_index(getpid());
   
//     for (int i = 0; i < LEVELS; i++) {
//         pid_t child_pid = fork();

//         if (child_pid > 0) {
//             // --- PARENT'S PATH ---
//             // The parent waits for its direct child to finish and then
//             // breaks from the loop, not creating any more children.
//             wait(NULL);
//             break;
//         } else if (child_pid == 0) {
//             // --- CHILD'S PATH ---
//             // The child registers the relationship to its parent.
//             add_edge_matrix(getppid(), getpid());
//             add_edge_list(getppid(), getpid());
            
//             // Print the current state of the graph from this child's perspective
//             printf("\n--- Graph state as seen by PID %d (Level %d) ---\n", getpid(), i + 1);
//             print_adjacency_list();
//             print_adjacency_matrix();
            
//             // This child will continue the loop to create its own child.
//         } else {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
//     }

//     // This message will be printed by every process in the chain
//     // as it exits the loop.
//     printf("Process %d finished its work.\n", getpid());

//     // The execlp and final wait are less relevant in this structure
//     // as the parent chain has already waited.
//     // execlp("ps", "ps", "-ef", NULL);

//     return 0;
// }