#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

#define MAX_INPUT_SIZE 1024
#define HISTORY_SIZE 5

using namespace std;

void print_prompt();
void parse_input(char *input, char **command);
void execute_command(char **command, vector<int>& history);
void help_command();
void history_command(const vector<int>& history);

int main() {
    char input[MAX_INPUT_SIZE];
    char *command[2];  
    vector<int> history;

    while (1) {
        print_prompt();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        parse_input(input, command);

        if (command[0] == NULL) {
            continue;  // Empty input
        }

        if (strcmp(command[0], "exit") == 0) {
            printf("Exit!\n");
            break;
        } else if (strcmp(command[0], "help") == 0) {
            help_command();
        } else if (strcmp(command[0], "history") == 0) {
            history_command(history);
        } else {
            execute_command(command, history);
        }
    }

    return 0;
}

void print_prompt() {
    printf("st582s$ ");  
}

// Note: This function should be replaced with the provided parsing code
void parse_input(char *input, char **command) {
    command[0] = strtok(input, " \n");
    command[1] = NULL;
}

void execute_command(char **command, std::vector<int>& history) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        if (execvp(command[0], command) == -1) {
            printf("Error: Command could not be executed\n");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        // Add child PID to history
        if (history.size() >= HISTORY_SIZE) {
            history.erase(history.begin());
        }
        history.push_back(pid);
    }
}

void help_command() {
    printf("//****************************************************\n");
    printf("//\n");
    printf("// <Sudarshan Tiwari!!!!>\n");
    printf("// CSC 360 Operating Systems\n");
    printf("// Project #1: My Own Shell\n");
    printf("// This shell supports the following commands: help, exit, history\n");
    printf("//\n");
    printf("//****************************************************\n");
}

void history_command(const vector<int>& history) {
    for (int i = 0; i < history.size(); i++) {
        printf("%d\n", history[i]);
    }
}