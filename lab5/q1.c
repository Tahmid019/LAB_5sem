#include "help.h"

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    printf("=====================================\n");
    printf("     Simple Shell  \n");
    printf("=====================================\n");
    printf("'exit' command to exit the shell.\n");

    while (1) {
        printPrompt();

        if (!takeInput(input))
            continue;

        if (strcmp(input, "exit") == 0) {
            printf("Exiting ... \n");
            break;
        }

        parseInput(input, args);

        executeCommand(args);
    }

    return 0;
}
