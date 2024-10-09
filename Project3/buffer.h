//********************************************************************
//
// Your Name
// Operating Systems
// Programming Project #3: Process Synchronization Using Pthreads
// October 8, 2024
// Instructor: Dr. Siming Liu
//
//********************************************************************

#ifndef _BUFFER_H_DEFINED_
#define _BUFFER_H_DEFINED_

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef int buffer_item;  // Buffer item type
#define BUFFER_SIZE 5     // Size of the buffer

// Declare the mutex and semaphores for synchronization
extern pthread_mutex_t mutex;
extern sem_t sem_empty;   // Renamed from 'empty' to 'sem_empty'
extern sem_t sem_full;    // Renamed from 'full' to 'sem_full'

// Function prototypes
void buffer_init();                          // Initialize buffer and synchronization tools
bool buffer_insert_item(buffer_item item);   // Insert an item into the buffer
bool buffer_remove_item(buffer_item *item);  // Remove an item from the buffer
bool is_prime(int n);                        // Check if a number is prime

#endif // _BUFFER_H_DEFINED_
