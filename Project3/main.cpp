#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>

#define BUFFER_SIZE 5
typedef int buffer_item;

// Buffer
buffer_item buffer[BUFFER_SIZE];
int front = 0, rear = 0, count = 0;

// Mutex and Semaphores
pthread_mutex_t mutex;
sem_t empty, full;

// Global variables
bool simulation_flag = true;  // Controls simulation end
int total_produced = 0, total_consumed = 0;
int max_sleep_time;
int show_snapshots = 0;

// Function declarations
int buffer_insert_item(buffer_item item);
int buffer_remove_item(buffer_item *item);
int is_prime(int num);
void *producer(void *param);
void *consumer(void *param);

int buffer_insert_item(buffer_item item) {
    if (count == BUFFER_SIZE) {
        return -1;  // Buffer is full
    }
    buffer[rear] = item;
    rear = (rear + 1) % BUFFER_SIZE;
    count++;
    return 0;
}

int buffer_remove_item(buffer_item *item) {
    if (count == 0) {
        return -1;  // Buffer is empty
    }
    *item = buffer[front];
    front = (front + 1) % BUFFER_SIZE;
    count--;
    return 0;
}

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void *producer(void *param) {
    while (simulation_flag) {
        sleep(rand() % max_sleep_time);  // Sleep for a random time

        buffer_item item = rand() % 100;  // Generate random item
        sem_wait(&empty);  // Wait if no empty slots
        pthread_mutex_lock(&mutex);  // Lock the buffer

        if (buffer_insert_item(item) == 0) {
            total_produced++;
            std::cout << "Producer " << pthread_self() << " produced " << item << "\n";
            if (show_snapshots) {
                std::cout << "(buffers occupied: " << count << ")\n";
            }
        } else {
            std::cout << "Buffer full, producer waits\n";
        }

        pthread_mutex_unlock(&mutex);  // Unlock the buffer
        sem_post(&full);  // Signal that an item was added
    }
    return NULL;  // Add this return statement
}

void *consumer(void *param) {
    while (simulation_flag) {
        sleep(rand() % max_sleep_time);  // Sleep for a random time

        buffer_item item;
        sem_wait(&full);  // Wait if buffer is empty
        pthread_mutex_lock(&mutex);  // Lock the buffer

        if (buffer_remove_item(&item) == 0) {
            total_consumed++;
            std::cout << "Consumer " << pthread_self() << " consumed " << item;
            if (is_prime(item)) {
                std::cout << " * * * PRIME * * *";
            }
            std::cout << "\n";
            if (show_snapshots) {
                std::cout << "(buffers occupied: " << count << ")\n";
            }
        } else {
            std::cout << "Buffer empty, consumer waits\n";
        }

        pthread_mutex_unlock(&mutex);  // Unlock the buffer
        sem_post(&empty);  // Signal that an item was removed
    }
    return NULL;  // Add this return statement
}


int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <simulation_time> <max_sleep_time> <num_producers> <num_consumers> <show_snapshots>\n";
        return -1;
    }

    // Command-line arguments
    int simulation_time = atoi(argv[1]);
    max_sleep_time = atoi(argv[2]);
    int num_producers = atoi(argv[3]);
    int num_consumers = atoi(argv[4]);
    show_snapshots = (std::string(argv[5]) == "yes") ? 1 : 0;

    // Initialize the buffer, mutex, and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially, BUFFER_SIZE slots are empty
    sem_init(&full, 0, 0);  // Initially, no items in the buffer

    // Create producer and consumer threads
    pthread_t producers[num_producers], consumers[num_consumers];
    for (int i = 0; i < num_producers; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    // Sleep for the simulation time
    sleep(simulation_time);

    // End the simulation
    simulation_flag = false;

    // Join threads and cleanup
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Display statistics
    std::cout << "\nPRODUCER / CONSUMER SIMULATION COMPLETE\n";
    std::cout << "=======================================\n";
    std::cout << "Simulation Time:\t\t" << simulation_time << "\n";
    std::cout << "Maximum Thread Sleep Time:\t" << max_sleep_time << "\n";
    std::cout << "Number of Producer Threads:\t" << num_producers << "\n";
    std::cout << "Number of Consumer Threads:\t" << num_consumers << "\n";
    std::cout << "Size of Buffer:\t\t\t" << BUFFER_SIZE << "\n";
    std::cout << "Total Number of Items Produced:\t" << total_produced << "\n";
    std::cout << "Total Number of Items Consumed:\t" << total_consumed << "\n";
    std::cout << "Number Of Items Remaining in Buffer:\t" << count << "\n";

    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
