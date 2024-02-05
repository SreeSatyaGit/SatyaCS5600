#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 512

void executeCommand(char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error forking process");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Error executing command");
            exit(EXIT_FAILURE);
        }
    }
    // Parent process does not wait, allows concurrent execution
}

int main(int argc, char *argv[]) {
    FILE *file = NULL;
    char input[MAX_INPUT_SIZE];

    // Check command line arguments
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [batchFile]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open batch file if provided
    if (argc == 2) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("Error opening batch file");
            exit(EXIT_FAILURE);
        }
    }

    // Main shell loop
    while (1) {
        if (file == NULL) {
            // Interactive mode
            printf("prompt> ");
            if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
                // End of input
                break;
            }
        } else {
            // Batch mode
            if (fgets(input, MAX_INPUT_SIZE, file) == NULL) {
                // End of batch file
                break;
            }
            printf("%s", input);  // Echo the command in batch mode
        }

        
        // Tokenize the input into commands separated by ';'
        char *token = strtok(input, ";");
        printf("Token: %s\n",token);
        while (token != NULL) {
            // Trim leading and trailing whitespaces
            while (*token == ' ' || *token == '\t') {
                token++;
            }
            size_t len = strlen(token);
            while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\t' || token[len - 1] == '\n')) {
                token[--len] = '\0';
            }
            // Skip empty commands
            if (strlen(token) > 0) {
                // Tokenize command arguments
                char *args[10];  // Assuming a maximum of 10 arguments
                int i = 0;
                args[i++] = strtok(token, " \t\n");
                while ((args[i++] = strtok(NULL, " \t\n")) != NULL && i < 10);

                // Execute the command
                executeCommand(args);
            }

            token = strtok(NULL, ";");
        }

        // Wait for all child processes to finish before prompting for the next input
        while (waitpid(-1, NULL, 0) > 0);
    }

    // Close batch file if opened
    if (file != NULL) {
        fclose(file);
    }

    return EXIT_SUCCESS;
}
