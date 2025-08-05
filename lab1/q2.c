#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct ProcessNode {
    pid_t pid;
    pid_t ppid;
    struct ProcessNode* left;
    struct ProcessNode* right;
} ProcessNode;

ProcessNode* create_node(pid_t pid) {
    ProcessNode* node = (ProcessNode*)malloc(sizeof(ProcessNode));
    if (!node) {
        perror("malloc");
        exit(1);
    }
    node->pid = pid;
    node->ppid = 0;
    node->left = node->right = NULL;
    return node;
}

void add_left_child(ProcessNode* parent, ProcessNode* child) {
    if (parent->left) return;
    parent->left = child;
    child->ppid = parent->pid;
}

void print_tree(ProcessNode* node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("    ");
    printf("-> %d (PPID: %d)\n", node->pid, node->ppid);
    print_tree(node->left, level + 1);
    print_tree(node->right, level + 1);
}

void free_tree(ProcessNode* node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main() {
    printf("\nleft skewed\n");

    
    if (fork() == 0) {
        ProcessNode* p1 = create_node(getpid());
        printf("p1 PID: %d\n", p1->pid);
        
        if (fork() == 0) {
            ProcessNode* p2 = create_node(getpid());
            add_left_child(p1, p2);
            printf("> p2 PID: %d\n", p2->pid);
            
            if (fork() == 0) {
                ProcessNode* p3 = create_node(getpid());
                add_left_child(p2, p3);
                printf(">> p3 P3 PID: %d\n", p3->pid);
                execlp("ps", "ps", "-ef", NULL);
                free_tree(p3);
                exit(0);
            } else {
                wait(NULL);
                free_tree(p2);
                exit(0);
            }
        } else {
            wait(NULL);
            free_tree(p1);
            exit(0);
        }
    } else {
        wait(NULL);
    }
    
    
    // execlp("ps", "ps", "-ef", NULL);
    return 0;
}
