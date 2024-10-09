#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "buffer.h"

int simulation_flag = 1; // Global flag to stop threads

void *producer(void *param) {
    int id = *((int*)param);
    buffer_item item;

    while (simulation_flag) {
        sleep(rand() % 3); // Sleep for a random time
        item = rand() % 100; // Produce a random item
        printf("Producer %d produced %d\n", id, item);

        if (buffer_insert_item(item)) {
            printf("Producer %d encountered an error\n", id);
        }
    }
    pthread_exit(NULL);
}

void *consumer(void *param) {
    int id = *((int*)param);
    buffer_item item;

    while (simulation_flag) {
        sleep(rand() % 3); // Sleep for a random time

        if (buffer_remove_item(&item) == 0) {
            printf("Consumer %d consumed %d\n", id, item);
            // Check for prime
            int is_prime = 1;
            if (item <= 1) is_prime = 0;
            for (int i = 2; i <= item / 2; i++) {
                if (item % i == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                printf("Consumer %d detected prime: %d\n", id, item);
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <sleep time> <max thread sleep> <# producers> <# consumers> <output buffer snapshots>\n", argv[0]);
        return -1;
    }

    // Command line arguments
    int main_sleep_time = atoi(argv[1]);
    int max_sleep_time = atoi(argv[2]);
    int num_producers = atoi(argv[3]);
    int num_consumers = atoi(argv[4]);
    int output_snapshots = (argv[5][0] == 'y');

    // Initialize buffer
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_t producers[num_producers], consumers[num_consumers];
    int thread_ids[num_producers + num_consumers];

    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &thread_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        thread_ids[num_producers + i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &thread_ids[num_producers + i]);
    }

    // Sleep for the specified simulation time
    sleep(main_sleep_time);

    // Stop simulation
    simulation_flag = 0;

    // Wait for all threads to finish
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Cleanup and print simulation stats
    printf("PRODUCER / CONSUMER SIMULATION COMPLETE\n");
    printf("Simulation Time: %d\n", main_sleep_time);
    printf("Buffer Size: %d\n", BUFFER_SIZE);

    // Destroy semaphores and mutex
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
