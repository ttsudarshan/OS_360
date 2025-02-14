#********************************************************************
#
# Sudarshan Tiwari
# CSC 360/660: Operating Systems
# Project #4: Implementation of Banker's Algorithm in pyhton
# October 21, 2024
# Instructor: Dr. Siming Liu
#
#********************************************************************



# matrixprinter
#
# This function is used to print a matrix (like allocation or max) in a 
# simple formatted way. It includes a title and labels for rows/columns 
# to make it easier to read.
#
# Parameters
# ----------
# title        str           The title shown before the matrix
# matrix       list[list[int]] The matrix that we need to print
# n            int           Number of processes (rows in the matrix)
# m            int           Number of resource types (columns in the matrix)
#
#********************************************************************

def matrixprinter(title, matrix, n, m):
    
    print(title)
    print("   A B C D")  # //taking 4 resources just like an exmple of output
    for i in range(n):
        print(f"{i}: ", end="")
        for j in range(m):
            print(f"{matrix[i][j]} ", end="")
        print()
    print()
#********************************************************************
#
# vectorprinter
#
# This function prints out a vector, which are availabe resources
#
# Parameters
# ----------
# title        str           The title displayed before the vector
# vec          list[int]     The vector we are printing
# m            int           Number of resource
#
#********************************************************************

def vectorprinter(title, vec, m):
    print(title)
    print("A B C D")  # //taking 4 resources just like an exmple of output
    for i in range(m):
        print(f"{vec[i]} ", end="")
    print("\n")
#********************************************************************
#
# needcalculator
#
# This function calculates the need matrix by subtracting allocation 
# from the max for each process and resource type.
#
# Parameters
# ----------
# need         int The matrix that will store the calculated need
# max_         int The max resources matrix
# allocation   int The current allocation matrix
# n            int           Number of processes
# m            int           Number of resource types
#
#********************************************************************
def needcalculator(need, max_, allocation, n, m):
    for i in range(n):
        for j in range(m):
            need[i][j] = max_[i][j] - allocation[i][j]
#********************************************************************
#
# safechecker
#
# This function checks if the system is in a safe state. It checks 
# if all processes can finish with the current resource availability.
#
# Parameters
# ----------
# available    list[int]     The currently available resources
# allocation   list[int] The allocated resources for each process
# need         list[int] The need matrix for each process
# n            int           Number of processes
# m            int           Number of resource types
#
# Returns
# -------
# bool         True if the system is in a safe state, False otherwise
#
#********************************************************************
def safechecker(available, allocation, need, n, m):
    finished = [False] * n
    work = available[:]
    count = 0

    while count < n:
        found = False
        for i in range(n):
            if not finished[i]:
                can_proceed = True
                for j in range(m):
                    if need[i][j] > work[j]:
                        can_proceed = False
                        break
                if can_proceed:
                    for j in range(m):
                        work[j] += allocation[i][j]
                    finished[i] = True
                    found = True
                    count += 1
        if not found:
            return False
    return True

#********************************************************************
#
# resourcerequester
#
# This function handles a resource request from a specific process. 
# It checks if the request can be granted and if the system would still  be in a safe state after granting it.
#
# Parameters
# ----------
# request      list[int]     The request vector from the process
# allocation   list[int] The current allocation matrix
# need         list[int] The need matrix for the process
# available    list[int]   The currently available resources
# process      int           The process making the request
# m            int           Number of resource types
#
# Returns
# -------
# bool         True if the request can be granted, False otherwise
#
#********************************************************************

def resourcerequester(request, allocation, need, available, process, m):
    # Check if request is within the need of the process
    for i in range(m):
        if request[i] > need[process][i]:
            return False
    
    # Check available resources
    for i in range(m):
        if request[i] > available[i]:
            return False
    
    # Temporarily allocated resources
    for i in range(m):
        available[i] -= request[i]
        allocation[process][i] += request[i]
        need[process][i] -= request[i]
    
    # Check if system is still in a safe state
    if safechecker(available, allocation, need, len(allocation), m):
        return True
    else:
        # Rollback allocation if not safe
        for i in range(m):
            available[i] += request[i]
            allocation[process][i] -= request[i]
            need[process][i] += request[i]
        return False


#main function
#********************************************
# THis handels file input and output request from the process and check if it is granted
#
#*********************************************
def main():
    input_file = input("Please enter the file name: ")

    try:
        with open(input_file, 'r') as file:
            lines = file.read().splitlines()
            
            lines = [line for line in lines if line.strip()]

            # Reading number of processes and resource types
            n = int(lines[0])
            m = int(lines[1])
            
            # Reads allocation matrix
            allocation = [list(map(int, lines[i + 2].split())) for i in range(n)]
            
            # Reads max matrix
            max_ = [list(map(int, lines[i + 2 + n].split())) for i in range(n)]
            
            # Reads available vector
            available = list(map(int, lines[2 + 2 * n].split()))
            
            # Reads process and request vector
            process_line = lines[3 + 2 * n].split(":")
            process = int(process_line[0])
            request = list(map(int, process_line[1].split()))
            #if file name is not found
    except FileNotFoundError:
        print("file not found")
        return
    
    # Output formatting
    print(f"There are {n} processes in the system.")
    print(f"There are {m} resource types.\n")

    matrixprinter("The Allocation Matrix is...", allocation, n, m)
    matrixprinter("The Max Matrix is...", max_, n, m)

    need = [[0] * m for _ in range(n)]
    needcalculator(need, max_, allocation, n, m)
    matrixprinter("The Need Matrix is...", need, n, m)
    vectorprinter("The Available Vector is...", available, m)

    safe = safechecker(available, allocation, need, n, m)
    if safe:
        print("THE SYSTEM IS IN A SAFE STATE!\n")
    else:
        print("THE SYSTEM IS NOT IN A SAFE STATE!\n")

    print("The Request Vector is...")
    print("  A B C D")
    print(f"{process}: ", end="")
    for i in request:
        print(f"{i} ", end="")
    print("\n")

    granted = resourcerequester(request, allocation, need, available, process, m)
    if granted:
        print("THE REQUEST CAN BE GRANTED!\n")
    else:
        print("THE REQUEST CANNOT BE GRANTED!\n")

    vectorprinter("The Available Vector is...", available, m)

if __name__ == "__main__":
    main()
