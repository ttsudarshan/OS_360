//********************************************************************
//
// Your Name
// Operating Systems
// Programming Project #3: Process Synchronization Using Pthreads
// October 8, 2024
// Instructor: Dr. Siming Liu
//
//********************************************************************

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>

// Global variables
pthread_mutex_t mutex;   // Mutex for critical sections
sem_t sem_empty;         // Renamed from 'empty' to 'sem_empty'
sem_t sem_full;          // Renamed from 'full' to 'sem_full'

buffer_item buffer[BUFFER_SIZE];  // Shared buffer array
int insert_index = 0;  // Index for inserting items into the buffer
int remove_index = 0;  // Index for removing items from the buffer

//********************************************************************
//
// Function: buffer_init
//
// This function initializes the mutex and semaphores used to
// synchronize access to the buffer.
//
//********************************************************************
void buffer_init() {
    pthread_mutex_init(&mutex, NULL);     // Initialize mutex
    sem_init(&sem_empty, 0, BUFFER_SIZE); // Initialize semaphore for empty slots (renamed)
    sem_init(&sem_full, 0, 0);            // Initialize semaphore for full slots (renamed)
}

//********************************************************************
//
// Function: buffer_insert_item
//
// This function inserts an item into the buffer.
//
// Return Value
// ------------
// bool       True if insertion was successful, False otherwise
//
// Value Parameters
// ----------------
// item       buffer_item    The item to be inserted
//
//********************************************************************
bool buffer_insert_item(buffer_item item) {
    sem_wait(&sem_empty);               // Wait if there are no empty slots (renamed)
    pthread_mutex_lock(&mutex);          // Enter critical section

    // Insert the item into the buffer
    buffer[insert_index] = item;
    insert_index = (insert_index + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);        // Exit critical section
    sem_post(&sem_full);                 // Signal that a new item is available (renamed)

    return true;  // Insertion is always successful in this implementation
}

//********************************************************************
//
// Function: buffer_remove_item
//
// This function removes an item from the buffer.
//
// Return Value
// ------------
// bool       True if removal was successful, False otherwise
//
// Reference Parameters
// --------------------
// item       buffer_item*   The item removed from the buffer
//
//********************************************************************
bool buffer_remove_item(buffer_item *item) {
    sem_wait(&sem_full);                // Wait if there are no full slots (renamed)
    pthread_mutex_lock(&mutex);          // Enter critical section

    // Remove the item from the buffer
    *item = buffer[remove_index];
    remove_index = (remove_index + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);        // Exit critical section
    sem_post(&sem_empty);                // Signal that a new empty slot is available (renamed)

    return true;  // Removal is always successful in this implementation
}

//********************************************************************
//
// Function: is_prime
//
// This function checks if a number is a prime number.
//
// Return Value
// ------------
// bool       True if the number is prime, False otherwise
//
// Value Parameters
// ----------------
// n          int            The number to be checked
//
//********************************************************************
bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}
