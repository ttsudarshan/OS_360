//********************************************************************
//
//
// Project #4: Implementation of Banker's Algorithm in C++
//
//********************************************************************
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

// Function to for formatting/printing matrix
//********************************************************************
//
// matrixprinter
//====================
// This function prints a matrix in a formatted way, with titles and labels
// for processes and resource types.
//
// Value Parameters
// =====================
// title        string          The title to be printed before the matrix
// matrix       vector<vector<int>>&   The matrix to be printed
// n            int             The number of processes
// m            int             The number of resource types
//
// Local Variables
// =========================
// i, j         int             Loop variables for traversing the matrix
//
//********************************************************************

void matrixprinter(const string& title, vector<vector<int>>& matrix, int n, int m) {
    cout << title << endl;
    cout << "   A B C D" << endl;  //taking 4 resources just like an exmple of output
    for (int i = 0; i < n; i++) {
        cout << i << ": ";
        for (int j = 0; j < m; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


// Function to formant/print vectors
//********************************************************************
//
// Function:: vectorprint
//
// This function prints a vector (representing resources) in a formatted
// way with a title and labels.
//
// Value Parameters
// ===========================
// title        string          The vector title to be printed before the vector
// vec          vector<int>&    The vector to be printed
// m            int             The length of the vector or we can say number of rescources of vector
//
// Local Variables
// ============================
// i            int             Loop variable for traversing the vector
//
//********************************************************************

void vectorprint(const string& title, vector<int>& vec, int m) {
    cout << title << endl;
    cout << "A B C D" << endl;  //taking 4 resources just like an exmple of output
    for (int i = 0; i < m; i++) {
        cout << vec[i] << " ";
    }
    cout << endl << endl;
}
// Function for calculating the need Matrix
//********************************************************************
//
// needcalculator
//
// This function calculates the need matrix, which represents how much more
// of each resource a process may need to complete its execution.
//
// Value Parameters
// ===============================
// n            int             The number of processes
// m            int             The number of resource types
//
// Reference Parameters
// ================================
// need         vector<vector<int>>&   The need matrix to be calculated
// max          vector<vector<int>>&   The maximum matrix thtats indicating maximum resources each process may request
// allocation   vector<vector<int>>&   The allocation matrix thats representing currently allocated resources to processes
//
// Local Variables
// ===============================
// i, j         int             Loop variables 
//
//********************************************************************

void needcalculator(vector<vector<int>>& need, vector<vector<int>>& max, vector<vector<int>>& allocation, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}
// Function to check if the system is safe or not
//********************************************************************
// Function: safechecker
//
// This function checks whether the system is in a safe state. It uses the
// Banker's Algorithm to simulate the resource allocation and ensure all
// processes can complete without leading to a deadlock.
//
// Return Value
// ===============================
// bool             True/False if Key is found
//
// Value Parameters
// ---------------- 
// n            int             The number of processes
// m            int             The number of resource types
//
// Reference Parameters
// ===============================
// available    vector<int>&    The vector of currently available resources
// allocation   vector<vector<int>>&   The allocation that is matrix of current resource allocations
// need         vector<vector<int>>&   The need matrix that is representing how much more each process requires
//
// Local Variables
// ==============================
// finished     vector<bool>    Flags to indicate which processes have finished
// work         vector<int>     simulates available resources
// count        int             keeps track the number of completed processes
// i, j         int             Loop variables
//
//********************************************************************

bool safechecker(vector<int>& available, vector<vector<int>>& allocation, vector<vector<int>>& need, int n, int m) {
    vector<bool> finished(n, false);
    vector<int> work = available;
    int count = 0;
    
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finished[i]) {
                bool canProceed = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }}
                if (canProceed) {
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                    finished[i] = true;
                    found = true;
                    count++;
                }}}
        if (!found) {
            return false;
        }
    }
    return true;
}
// this  handles resource request
//********************************************************************
//
// resourcerequestor
//
// This function handles a resource request by a process. It checks if the request can be granted without 
// making the system unsafe, and temporarily allocates resources to simulate the system state.
//
// Return Value
// ==================================
// bool         True if the request can be safely granted, false otherwise
//
// Value Parameters
// ===================================
// process      int             The ID of the process which makes request
// m            int             The number of resource types
//
// Reference Parameters
// ===================================
// request      vector<int>&    The vector representing the resources requested by the process
// allocation   vector<vector<int>>&   The allocation matrix thats going to get updated only if the request is granted
// need         vector<vector<int>>&   The need matrix thats going to get updated only if  the request is granted
// available    vector<int>&    The available resources thats going to get updated only if the request is granted
//
// Local Variables
// ===================================
// i            int             Loop variable f
// Proceed   bool             True/False if the process can proceed based on available resources
//
//*******************************************************************
bool resourcerequestor(vector<int>& request, vector<vector<int>>& allocation, vector<vector<int>>& need, vector<int>& available, int process, int m) {
    // Check if request is within the need of the process
    for (int i = 0; i < m; i++) {
        if (request[i] > need[process][i]) {
            return false;
        }
    }
    
    // Checks available resources
    for (int i = 0; i < m; i++) {
        if (request[i] > available[i]) {
            return false;
        }
    }

    // Temporarily allocate the resources and checks the system wheter safe or not
    for (int i = 0; i < m; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }
    
    // Checking if system is still in safe state
    if (safechecker(available, allocation, need, allocation.size(), m)) {
        return true;
    } else {
        // Rolles back allocation
        for (int i = 0; i < m; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }
    
    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error opening file." << endl;
        return 1;
    }
    
    int n, m;
    file >> n >> m; // Read number of processes and resource types

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> max(n, vector<int>(m));
    vector<int> available(m);
    vector<int> request(m);
    vector<vector<int>> need(n, vector<int>(m));

    // Read allocation matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            file >> allocation[i][j];
        }
    }

    // Read max matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            file >> max[i][j];
        }
    }

    // Read available vector
    for (int i = 0; i < m; i++) {
        file >> available[i];
    }

    // Read request vector
    int process;
    char colon;
    file >> process >> colon; // Read process number and the colon 
    for (int i = 0; i < m; i++) {
        file >> request[i]; // Read the actual request vector
    }
    

    //
    //Formatting for the output 
    //trying to make it look like an eaxample
    //    
    //    
    //    
    // printing the number of processes and resource types
    cout << "There are " << n << " processes in the system." << endl;
    cout << "There are " << m << " resource types." << endl << endl;

    // printing the Allocation Matrix
    matrixprinter("The Allocation Matrix is...", allocation, n, m);

    // printing the max Matrix
    matrixprinter("The Max Matrix is...", max, n, m);

    // Calculates the need Matrix
    needcalculator(need, max, allocation, n, m);

    // prints the need matrix
    matrixprinter("The Need Matrix is...", need, n, m);

    // prints the available vector
    vectorprint("The Available Vector is...", available, m);

    //the below funcion is used to check the system saftey
    bool safe = safechecker(available, allocation, need, n, m);
    if (safe) {
        cout << "THE SYSTEM IS IN A SAFE STATE!" << endl << endl;
    } else {
        cout << "THE SYSTEM IS NOT IN A SAFE STATE!" << endl << endl;
    }

    // printing the Request Vector
    cout << "The Request Vector is..." << endl;
    cout << "  A B C D" << endl;
    cout << process << ":";
    for (int i = 0; i < m; i++) {
        cout << request[i] << " ";
    }
    cout << endl << endl;

    // Process the request and prints the result
    bool granted = resourcerequestor(request, allocation, need, available, process, m);
    if (granted) {
        cout << "THE REQUEST CAN BE GRANTED!" << endl << endl;
    } else {
        cout << "THE REQUEST CANNOT BE GRANTED!" << endl << endl;
    }

    // prints the new Available Vector
    vectorprint("The Available Vector is...", available, m);
    
    return 0;
}
