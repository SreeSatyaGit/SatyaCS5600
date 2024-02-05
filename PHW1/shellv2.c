#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 512

int splitString(const char *input, char *delimiter, char ***tokens) {
    // Make a copy of the input string to avoid modifying the original
    char *inputCopy = strdup(input);
    if (inputCopy == NULL) {
        perror("Error duplicating input string");
        exit(EXIT_FAILURE);
    }

    // Initialize variables
    int count = 0;
    char *token = strtok(inputCopy, delimiter);

    // Count the number of tokens
    while (token != NULL) {
        count++;
        token = strtok(NULL, delimiter);
    }

    // Allocate memory for the array of tokens
    *tokens = (char **)malloc(count * sizeof(char *));
    if (*tokens == NULL) {
        perror("Error allocating memory for tokens");
        exit(EXIT_FAILURE);
    }

    // Copy tokens into the array
    strcpy(inputCopy, input);
    token = strtok(inputCopy, delimiter);
    for (int i = 0; i < count; i++) {
        (*tokens)[i] = strdup(token);
        if ((*tokens)[i] == NULL) {
            perror("Error duplicating token");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, delimiter);
    }

    // Free the duplicated input string
    free(inputCopy);

    return count;
}



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

    char *delimiter = ";";
    char **tokens;
    
    int count = splitString(input, delimiter, &tokens);

    for (int i = 0; i < count; i++) {
        char *args[10];  // Assuming a maximum of 10 arguments
        int j = 0;
        args[j++] = strtok(tokens[i], " \t\n");
        while ((args[j++] = strtok(NULL, " \t\n")) != NULL && j < 10);

        // Execute the command
        executeCommand(args);
        
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
