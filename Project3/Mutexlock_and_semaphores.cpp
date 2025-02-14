/*****************************/
//Build by: Sudarshan Tiwari
//
//Project3 or Osproj4
//mutex and semaphores
//**************************//
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <iomanip>

#define BUFFER_SIZE 5
#define EMPTY -1

typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];

// Mutex and semaphores
pthread_mutex_t mutex;
sem_t sem_empty, full;

// Global variables 
using namespace std;
bool buffer_snapshots = false;
int items_produced = 0, items_consumed = 0;
int buffer_full_count = 0, buffer_empty_count = 0;
int producers_done = 0, consumers_done = 0;
int write_pos = 0, read_pos = 0;

// function to print the buffer state
void print_buffer() {
    cout << "(buffers occupied: " << items_produced - items_consumed << ")\n";
    cout << "buffers: ";
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (buffer[i] == EMPTY)
            cout << "  -1 ";
        else
            cout << setw(4) << buffer[i] << " ";
    }
    cout << "\n         "; 
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        cout << "----" << (i < BUFFER_SIZE - 1 ? "-" : "");
    }
    cout << "\n         "; 
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (i == read_pos && i == write_pos)
            cout << " RW  ";
        else if (i == read_pos)
            cout << " R   ";
        else if (i == write_pos)
            cout << " W   ";
        else
            cout << "     ";
    }
    cout << "\n";
}

// Checking the prime number
bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0)
            return false;
    }
    return true;
}

// Inserting item into buffer
bool insert_item(buffer_item item) {
    sem_wait(&sem_empty);
    //aquire mutex lock
    pthread_mutex_lock(&mutex);

    if ((items_produced - items_consumed) == BUFFER_SIZE) {
          //release mutex lock
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        return false; // Buffer full
    }

    buffer[write_pos] = item;
    write_pos = (write_pos + 1) % BUFFER_SIZE;
    items_produced++;
    //release mutex lock
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return true;
}

// Removeing item from buffer
bool remove_item(buffer_item* item) {
    sem_wait(&full);
    //aquire mutex lock
    pthread_mutex_lock(&mutex);

    if (items_produced == items_consumed) {
          //release mutex lock
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty);
        return false; // Buffer empty
    }

    *item = buffer[read_pos];
    buffer[read_pos] = EMPTY;
    read_pos = (read_pos + 1) % BUFFER_SIZE;
    items_consumed++;
    //release mutex lock
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_empty);
    return true;
}


// Producer function
void* producer(void* param) {
    long tid = (long)param;
    buffer_item item;
    while (producers_done == 0) {
        sleep(rand() % 3 + 1); // Random sleep
        item = rand() % 100;

        if (insert_item(item)) {
            cout << "Producer " << tid << " writes " << item << "\n";
            if (buffer_snapshots) {
                print_buffer();
            }
        } else {
            cout << "All buffers full. Producer " << tid << " waits.\n";
            buffer_full_count++;
        }
    }
    return NULL;
}

// Consumer function
void* consumer(void* param) {
    long tid = (long)param;
    buffer_item item;
    while (consumers_done == 0) {
        sleep(rand() % 3 + 1); // Random sleep

        if (remove_item(&item)) {
            cout << "Consumer " << tid << " reads " << item;
            if (is_prime(item)) {
                cout << "   * * * PRIME * * *\n";
            } else {
                cout << "\n";
            }
            if (buffer_snapshots) {
                print_buffer();
            }
        } else {
            cout << "All buffers empty. Consumer " << tid << " waits.\n";
            buffer_empty_count++;
        }
    }
    return NULL;
}


// Main function
int main(int argc, char* argv[]) {
  buffer_snapshots = (string(argv[5]) == "yes");

    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " <sim_time> <max_sleep_time> <num_producers> <num_consumers> <buffer_snapshots>\n";
        return -1;
    }

    int sim_time = atoi(argv[1]);
    int max_sleep_time = atoi(argv[2]);
    int num_producers = atoi(argv[3]);
    int num_consumers = atoi(argv[4]);
    bool buffer_snapshots = (string(argv[5]) == "yes");

    // Initializing buffer
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = EMPTY;
    }

    // Initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Create producer and consumer threads
    vector<pthread_t> producers(num_producers);
    vector<pthread_t> consumers(num_consumers);

    cout << "Starting Threads...\n";
    print_buffer();

    for (int i = 0; i < num_producers; ++i) {
        pthread_create(&producers[i], NULL, producer, (void*)(long)(i + 1));
    }

    for (int i = 0; i < num_consumers; ++i) {
        pthread_create(&consumers[i], NULL, consumer, (void*)(long)(i + 1));
    }

    // runs simulation according to the user command
    sleep(sim_time);

    // Signaling all threads to terminate
    producers_done = 1;
    consumers_done = 1;

    // Wait for all threads to complete
    for (int i = 0; i < num_producers; ++i) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(consumers[i], NULL);
    }

    // Displaying final statistics
    cout << "\nPRODUCER / CONSUMER SIMULATION COMPLETE\n";
    cout << "=======================================\n";
    cout << "Simulation Time:            " << sim_time << "\n";
    cout << "Maximum Thread Sleep Time:  " << max_sleep_time << "\n";
    cout << "Number of Producer Threads: " << num_producers << "\n";
    cout << "Number of Consumer Threads: " << num_consumers << "\n";
    cout << "Size of Buffer:             " << BUFFER_SIZE << "\n\n";
    cout << "Total Number of Items Produced:   " << items_produced << "\n";
    cout << "Total Number of Items Consumed:   " << items_consumed << "\n\n";
    cout << "Number Of Items Remaining in Buffer: " << (items_produced - items_consumed) << "\n";
    cout << "Number Of Times Buffer Was Full:   " << buffer_full_count << "\n";
    cout << "Number Of Times Buffer Was Empty:  " << buffer_empty_count << "\n";

    // Clean up
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_empty);
    sem_destroy(&full);

    return 0;
}
