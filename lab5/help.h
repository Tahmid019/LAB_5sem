#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

void printPrompt() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\n%s$ ", cwd);
}

int takeInput(char *input) {
    if (fgets(input, MAX_INPUT, stdin) == NULL)
        return 0;

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0)
        return 0;

    return 1;
}

int parseInput(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " ");

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    args[i] = NULL; 
    return i;
}
void executeCommand(char **args) {
    pid_t pid;
    int status;

    pid = fork(); 

    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            printf("Invalid command: %s\n", args[0]);
        }
        exit(0);
    }
    else {
        waitpid(pid, &status, 0);
    }
}


f[0] = true
t = 1
wait(f[])
