#include <unistd.h>

int main() {
    char *args[] = {"ls", "-l", NULL};
    execvp("ls", args);

    // If execvp fails, the code below will be executed
    perror("execvp");
    return 1;
}
