#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to split a string into an array of substrings
// Returns the number of tokens found
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

// Function to free memory allocated for tokens
void freeTokens(char ***tokens, int count) {
    for (int i = 0; i < count; i++) {
        free((*tokens)[i]);
    }
    free(*tokens);
}

int main() {
    const char *input = "ls;;date;;   ls -l";
    char *delimiter = ";";
    char **tokens;
    
    int count = splitString(input, delimiter, &tokens);

    printf("Number of tokens: %d\n", count);

    for (int i = 0; i < count; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }

    // Free allocated memory
    freeTokens(&tokens, count);

    return 0;
}
