#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <cstring>

using namespace std;

const string BEAR_ID = "st582s"; 


vector<pid_t> history;


void phelp();
void executeCommand(const string& command);
void printHistory();
void History(pid_t pid);

int main() {
    string input;

    while (true) {
        // Print prompt
        cout << BEAR_ID << "$ ";
        getline(cin, input);

        if (input == "exit") {
            cout << "Exit!" << endl;
            break;
        } else if (input == "help") {
            phelp();
        } else if (input == "history") {
            printHistory();
        } else {
            executeCommand(input);
        }
    }

    return 0;
}

void phelp() {
    cout << "//*********************************************************" << endl;
    cout << "//" << endl;
    cout << "// <Sudarshan Tiwari!!!!>" << endl;
    cout << "// CSC360 Operating Systems" << endl;
    cout << "// Project #1: My Shell - Writing Your Own Shell" << endl;
    cout << "// This shell supports the following commands: help, exit, history" << endl;
    cout << "//" << endl;
    cout << "//*********************************************************" << endl;
}

void executeCommand(const string& command) {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return;
    }

    if (pid == 0) { // Child procss
        execl("/bin/sh", "sh", "-c", command.c_str(), (char*)nullptr);
        // If afils
        cerr << "Error: Command could not be executed" << endl;
        exit(1);
    } else { // Praent process
        History(pid);
        int status;
        waitpid(pid, &status, 0);
    }
}

void printHistory() {
    for (auto pid : history) {
        cout << pid << endl;
    }
}

void History(pid_t pid) {
    if (history.size() >= 5) {
        history.erase(history.begin());
    }
    history.push_back(pid);
}
