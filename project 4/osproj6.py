#********************************************************************
#
# Sudarshan Tiwari
# CSC 360/660: Operating Systems
# Project #4: Implementation of Simulation of Page Replacement Strategies
# October 30, 2024
# Instructor: Dr. Siming Liu
#
#********************************************************************


import random
#==============================================================
# fifo
#
# This function implements the FIFO page replacement algorithm. 
# It counts the number of page faults that occur when processing 
# the given page reference string with the specified number of frames.
#
# Parameters
# ----------
# referencestring  list[int]     The list of page references
# numframes        int            The number of frames available
#
# 
# -------
# int              The total number of page faults encountered
#
#==================================================
def fifo(referencestring, frames):
    pagefaults = 0
    memory = []
    for page in referencestring:
        if page not in memory:
            pagefaults += 1
            if len(memory) >= frames:
                memory.pop(0)  # Remove the oldest page
            memory.append(page)
    return pagefaults

#===============================================================
#
# lru
#
# This function implements the LRU page replacement algorithm. 
# It counts the number of page faults that occur when processing 
# the given page reference string with the specified number of frames.
#
# Parameters
# ----------
# referencestring  list[int]     The list of page references
# numframes        int            The number of frames available
#
# Returns
# -------
# int              The total number of page faults encountered
#
#=================================================================
def lru(referencestring, frames):
    pagefaults = 0
    memory = []
    pageindices = {}
    for i, page in enumerate(referencestring):
        if page not in memory:
            pagefaults += 1
            if len(memory) >= frames:
                # Find least recently used
                lru_page = min(pageindices, key=pageindices.get)
                memory.remove(lru_page)
                del pageindices[lru_page]
            memory.append(page)
        pageindices[page] = i  # Update the index of the page
    return pagefaults


#===================================================================
#
# pagefaults_opt
#
# This function implements the Optimal page replacement algorithm. 
# It counts the number of page faults that occur when processing 
# the given page reference string with the specified number of frames.
#
# Parameters
# ----------
# referencestring  list[int]     The list of page references
# frames        int            The number of frames available
#
# Returns
# -------
# int              The total number of page faults encountered
#=======================================================
def optimal(referencestring, frames):
    pagefaults = 0
    memory = []
    for i, page in enumerate(referencestring):
        if page not in memory:
            pagefaults =pagefaults+ 1
            if len(memory) >= frames:
                # Find the page to replace
                furthest = -1
                pagetoremove = None
                for mempage in memory:
                    try:
                        next_use = referencestring[i + 1:].index(mempage)
                    except ValueError:
                        next_use = float('inf')  # Page not used again
                    if next_use > furthest:
                        furthest = next_use
                        pagetoremove = mempage
                memory.remove(pagetoremove)
            memory.append(page)
    return pagefaults

#===========================================
# This function implements the Random page replacement algorithm. 
# It counts the number of page faults that occur when processing 
# the given page reference string with the specified number of frames.
#parameters are same as others
#==============================================
def rand(referencestring, frames):
    pagefaults = 0
    memory = []
    for page in referencestring:
        if page not in memory:
            pagefaults =pagefaults+ 1
            if len(memory) >= frames:
                memory.pop(random.randint(0, len(memory) - 1))  # Remove a random page
            memory.append(page)
    return pagefaults
#main function
def main():

    filename = input("Enter the input file name: ")
    
    try:
        with open(filename, 'r') as file:
            # Reads input
            referencestring = list(map(int, file.readline().strip().split()))
            frames = int(file.readline().strip())
            algorithms = [line.strip() for line in file.readlines()]

        # Output the reference string and number of frames
        print("Page Reference String:")
        for i in range(0, len(referencestring), 20):
            print(" ".join(map(str, referencestring[i:i + 20])))
        print(f"Number of Frames: {frames}")

        # Calculate and output page faults for each algorithm
        for algo in algorithms:
            if algo == "FIFO":
                faults = fifo(referencestring, frames)
                print(f"FIFO: {faults}")
            elif algo == "LRU":
                faults = lru(referencestring, frames)
                print(f"LRU: {faults}")
            elif algo == "OPT":
                faults = optimal(referencestring, frames)
                print(f"OPT: {faults}")
            elif algo == "RAND":
                faults = rand(referencestring, frames)
                print(f"RAND: {faults}")
    except FileNotFoundError:
        print(f"Error: The file '{filename}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
