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
    parent->left = child;
    child->ppid = parent->pid;
}

void add_right_child(ProcessNode* parent, ProcessNode* child) {
    parent->right = child;
    child->ppid = parent->pid;
}

void print_tree(ProcessNode* node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("    ");
    if (level == 0)
        printf("-> %d (Root)\n", node->pid);
    else
        printf("-> %d (Child of %d)\n", node->pid, node->ppid);
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
    printf("main PID: %d\n", getpid());

    ProcessNode* root = create_node(getpid());

    pid_t pid1 = fork();
    if (pid1 == 0) {
        printf("left = %d || par = %d \n", getpid(), getppid());
        sleep(5);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("right = %d || par = %d \n", getpid(), getppid());
        sleep(5);
        exit(0);
    }

    execlp("ps", "ps", "-ef", NULL);

    wait(NULL);

    ProcessNode* left_child = create_node(pid1);
    ProcessNode* right_child = create_node(pid2);

    add_left_child(root, left_child);
    add_right_child(root, right_child);

    printf("\ndisplay process Tree:\n");
    print_tree(root, 0);

    free_tree(root);
    return 0;
}
