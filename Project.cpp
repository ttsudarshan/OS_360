#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

#define MAX_INPUT_SIZE 1024
#define MAX_HISTORY 5

void execute_command(char *command, std::vector<pid_t> &history);
void print_history(const std::vector<pid_t> &history);

int main() {
    char input[MAX_INPUT_SIZE];
    char *command;
    std::vector<pid_t> history;

    while (1) {
        // Part 1: Display the prompt with BearID
        printf("<YourBearID>$ ");
        
        // Get user input
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Part 3: Handle exit command
        if (strcmp(input, "exit") == 0) {
            printf("Exit!\n");
            break;
        }

        // Part 4: Handle built-in commands
        if (strcmp(input, "help") == 0) {
            printf("//*********************************************************\n");
            printf("// Your Name Here\n");
            printf("// CSC360/660 Operating Systems\n");
            printf("// Project #2: My Shell - Writing Your Own Shell\n");
            printf("// This shell supports the following commands: help exit history\n");
            printf("//*********************************************************\n");
            continue;
        } else if (strcmp(input, "history") == 0) {
            print_history(history);
            continue;
        }

        // Execute the command
        execute_command(input, history);
    }

    return 0;
}

void execute_command(char *command, std::vector<pid_t> &history) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
    } else if (pid == 0) {
        // Child process
        execlp(command, command, NULL);
        // If execlp returns, it must have failed
        perror("Error: Command could not be executed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        waitpid(pid, NULL, 0);

        // Record child process ID in history
        if (history.size() >= MAX_HISTORY) {
            history.erase(history.begin());
        }
        history.push_back(pid);
    }
}

void print_history(const std::vector<pid_t> &history) {
    for (size_t i = 0; i < history.size(); i++) {
        printf("%d\n", history[i]);
    }
}
