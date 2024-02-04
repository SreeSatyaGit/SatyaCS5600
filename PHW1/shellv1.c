#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void execute_command(char *command, char *arguments[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execvp(command, arguments);
        perror("execvp");  // This will be printed only if execvp fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process terminated abnormally\n");
        }
    }
}

int main() {
    char input[MAX_COMMAND_LENGTH];
    char *arguments[MAX_ARGUMENTS];

    while (1) {
        printf("Prompt ('kill' to quit): ");
        fgets(input, sizeof(input), stdin);

        // Remove newline character at the end of the input
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Exit the shell if the user enters 'exit'
        if (strcmp(input, "kill") == 0) {
            break;
        }

        // Tokenize the input into command and arguments
        char *token = strtok(input, " ");
        int i = 0;

        while (token != NULL && i < MAX_ARGUMENTS - 1) {
            arguments[i++] = token;
            token = strtok(NULL, " ");
        }

        arguments[i] = NULL;  // Null-terminate the arguments array

        // Execute the command
        if (i > 0) {
            execute_command(arguments[0], arguments);
        }
    }

    printf("Exiting the shell. Goodbye!\n");

    return 0;
}
