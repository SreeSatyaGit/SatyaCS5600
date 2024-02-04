#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void foo(char *arguments[]) {
    int num1 = atoi(arguments[1]);
    int num2 = atoi(arguments[2]);

    printf("Sum of %d and %d is: %d\n", num1, num2, num1 + num2);
}

void execute_command(char *command, char *arguments[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (strcmp(command, "foo") == 0) {
            // Call the custom function for 'foo'
            foo(arguments);
            exit(EXIT_SUCCESS);
        } else {
            execvp(command, arguments);
            perror("execvp");  // This will be printed only if execvp fails
            exit(EXIT_FAILURE);
        }
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
    char *commands[MAX_ARGUMENTS];  // Changed the array name to 'commands'

    while (1) {
        printf("Prompt ('dead' to quit): ");
        fgets(input, sizeof(input), stdin);

        // Remove newline character at the end of the input
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Exit the shell if the user enters 'dead'
        if (strcmp(input, "dead") == 0) {
            break;
        }

        // Tokenize the input into commands using semicolon as delimiter
        char *token = strtok(input, ";");

        while (token != NULL) {

            
            // Tokenize each command into command and arguments using space as delimiter
            char *cmd_token = strtok(token, " ");
            int i = 0;

            while (cmd_token != NULL && i < MAX_ARGUMENTS - 1) {
                commands[i++] = cmd_token;
                cmd_token = strtok(NULL, " ");
            }

            commands[i] = NULL;  // Null-terminate the commands array

            // Execute the command
            if (i > 0) {
                execute_command(commands[0], commands);
            }

            // Move to the next command
            
        }
        token = strtok(NULL, " ");
    }

    printf("Exiting the shell. Goodbye!\n");

    return 0;
}
