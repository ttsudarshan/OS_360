#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long long *fib_sequence; // Pointer to an array of long long
  int sequence_size;
} shared_data;

shared_data *shared_memory;

void generate_fibonacci(int sequence_size) {
  if (sequence_size < 1) return;

  shared_memory->fib_sequence[0] = 0;
  if (sequence_size > 1) {
    shared_memory->fib_sequence[1] = 1;
  }

  for (int i = 2; i < sequence_size; i++) {
    shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i - 1] + shared_memory->fib_sequence[i - 2];
  }
}

void *thread_function(void *args) {
  generate_fibonacci(shared_memory->sequence_size);
  return NULL;
}

int main(void) {
  int sequence_size;

  printf("Enter the number of Fibonacci numbers to generate: ");
  scanf("%d", &sequence_size);

  shared_memory = malloc(sizeof(shared_data));
  shared_memory->fib_sequence = malloc(sizeof(long long) * sequence_size);
  shared_memory->sequence_size = sequence_size;

  pthread_t thread;

  pthread_create(&thread, NULL, thread_function, NULL);
  pthread_join(thread, NULL);

  for (int i = 0; i < shared_memory->sequence_size; i++) {
    printf("%lld ", shared_memory->fib_sequence[i]);
  }

  free(shared_memory->fib_sequence);
  free(shared_memory);

  return 0;
}
