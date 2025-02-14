#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

#define BUFFER_SIZE 5
#define EMPTY -1

typedef int buffer_item;

// Global variables
buffer_item buffer[BUFFER_SIZE];
int buffer_in = 0;
int buffer_out = 0;
int items_produced = 0;
int items_consumed = 0;
int buffer_full_count = 0;
int buffer_empty_count = 0;
bool producers_done = false;
bool consumers_done = false;
bool buffer_snapshots = false;

// Mutex and semaphores
pthread_mutex_t mutex;
sem_t sem_full;
sem_t sem_free_slots;  // Renamed to avoid ambiguity

// Function declarations
void* producer(void* param);
void* consumer(void* param);
void print_buffer();
bool is_prime(int num);

// Buffer manipulation functions
int insert_item(buffer_item item) {
    sem_wait(&sem_free_slots);
    pthread_mutex_lock(&mutex);

    if (buffer_in == buffer_out && buffer[buffer_in] != EMPTY) {
        buffer_full_count++;
    }

    buffer[buffer_in] = item;
    buffer_in = (buffer_in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&sem_full);

    return 0;
}

int remove_item(buffer_item* item) {
    sem_wait(&sem_full);
    pthread_mutex_lock(&mutex);

    if (buffer_in == buffer_out && buffer[buffer_out] == EMPTY) {
        buffer_empty_count++;
    }

    *item = buffer[buffer_out];
    buffer[buffer_out] = EMPTY;
    buffer_out = (buffer_out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&sem_free_slots);

    return 0;
}

// Check if a number is prime
bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

// Print the current state of the buffer
void print_buffer() {
    cout << "(buffers occupied: " << ((buffer_in + BUFFER_SIZE - buffer_out) % BUFFER_SIZE) << ")\n";
    cout << "buffers:  ";
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (buffer[i] == EMPTY)
            cout << " - ";
        else
            cout << buffer[i] << " ";
    }
    cout << "\n";
}

// Producer thread function
void* producer(void* param) {
    int id = (long)param;
    while (!producers_done) {
        sleep(rand() % 3 + 1);  // Sleep for a random time (1-3 seconds)

        buffer_item item = rand() % 100;  // Produce a random item
        if (insert_item(item) == 0) {
            items_produced++;
            cout << "Producer " << id << " produced " << item << endl;

            if (buffer_snapshots) {
                print_buffer();
            }
        }
    }
    pthread_exit(0);
}

// Consumer thread function
void* consumer(void* param) {
    int id = (long)param;
    while (!consumers_done) {
        sleep(rand() % 3 + 1);  // Sleep for a random time (1-3 seconds)

        buffer_item item;
        if (remove_item(&item) == 0) {
            items_consumed++;
            cout << "Consumer " << id << " consumed " << item;

            if (is_prime(item)) {
                cout << "   * * * PRIME * * *";
            }

            cout << endl;

            if (buffer_snapshots) {
                print_buffer();
            }
        }
    }
    pthread_exit(0);
}

// Main function
int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " <sim_time> <max_sleep_time> <num_producers> <num_consumers> <buffer_snapshots>\n";
        return -1;
    }

    int sim_time = atoi(argv[1]);
    int max_sleep_time = atoi(argv[2]);
    int num_producers = atoi(argv[3]);
    int num_consumers = atoi(argv[4]);
    buffer_snapshots = (string(argv[5]) == "yes");

    // Initialize the buffer to EMPTY
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = EMPTY;
    }

    // Initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_free_slots, 0, BUFFER_SIZE);  // Initialize with buffer capacity
    sem_init(&sem_full, 0, 0);  // Initialize with zero full slots

    // Create producer and consumer threads
    vector<pthread_t> producers(num_producers);
    vector<pthread_t> consumers(num_consumers);

    cout << "Starting Threads...\n";
    if (buffer_snapshots) {  // Print initial buffer if snapshots are enabled
        print_buffer();
    }

    // Start producer threads
    for (int i = 0; i < num_producers; ++i) {
        pthread_create(&producers[i], NULL, producer, (void*)(long)(i + 1));
    }

    // Start consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        pthread_create(&consumers[i], NULL, consumer, (void*)(long)(i + 1));
    }

    // Run the simulation for the given time
    sleep(sim_time);

    // Signal the threads to terminate
    producers_done = true;
    consumers_done = true;

    // Wait for all threads to complete
    for (int i = 0; i < num_producers; ++i) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(consumers[i], NULL);
    }

    // Display final statistics
    cout << "\nPRODUCER / CONSUMER SIMULATION COMPLETE\n";
    cout << "=======================================\n";
    cout << "Simulation Time:            " << sim_time << " seconds\n";
    cout << "Maximum Thread Sleep Time:  " << max_sleep_time << " seconds\n";
    cout << "Number of Producer Threads: " << num_producers << "\n";
    cout << "Number of Consumer Threads: " << num_consumers << "\n";
    cout << "Size of Buffer:             " << BUFFER_SIZE << "\n\n";
    cout << "Total Number of Items Produced:   " << items_produced << "\n";
    cout << "Total Number of Items Consumed:   " << items_consumed << "\n\n";
    cout << "Number Of Items Remaining in Buffer: " << (items_produced - items_consumed) << "\n";
    cout << "Number Of Times Buffer Was Full:   " << buffer_full_count << "\n";
    cout << "Number Of Times Buffer Was Empty:  " << buffer_empty_count << "\n";

    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_free_slots);
    sem_destroy(&sem_full);

    return 0;
}
