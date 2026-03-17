#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Shared Memory and Buffer Constants
#define BUFFER_SIZE 100
#define PAIR_CAPACITY (BUFFER_SIZE / 2)

int particle_buffer[BUFFER_SIZE];
int in = 0;  // Index for producers
int out = 0; // Index for consumer

// Semaphores
sem_t empty;  // Counts available pair slots (starts at 50)
sem_t full;   // Counts available pairs for consumer (starts at 0)
sem_t mutex;  // Binary semaphore for producer mutual exclusion

// Producer Thread Function
void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        // Produce pair P1, P2 (simulated by IDs)
        int p1 = id * 10 + 1;
        int p2 = id * 10 + 2;

        sem_wait(&empty); // Rule 1: Wait for 2 spaces (1 pair slot)
        sem_wait(&mutex); // Rule 2: Ensure P1 and P2 are consecutive

        // Place P1 and P2 in buffer
        particle_buffer[in] = p1;
        in = (in + 1) % BUFFER_SIZE;
        particle_buffer[in] = p2;
        in = (in + 1) % BUFFER_SIZE;

        printf("Producer %d: Placed pair (%d, %d)\n", id, p1, p2);

        sem_post(&mutex);
        sem_post(&full);  // Signal that a pair is ready

        sleep(1); // Simulate production time
    }
    return NULL;
}

// Consumer Thread Function
void* consumer(void* arg) {
    while (1) {
        sem_wait(&full); // Rule 4: Only proceed if buffer is NOT empty

        // Fetch P1 and P2 from buffer
        int p1 = particle_buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        int p2 = particle_buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        printf("Consumer: Fetched pair (%d, %d) and shipped.\n", p1, p2);

        sem_post(&empty); // Signal that a pair slot is now free

        sleep(2); // Simulate packaging/shipping time
    }
    return NULL;
}

int main() {
    pthread_t prod_threads[3], cons_thread;
    int prod_ids[3] = {1, 2, 3};

    // Initialize Semaphores
    sem_init(&empty, 0, PAIR_CAPACITY); // 50 pair slots
    sem_init(&full, 0, 0);               // 0 pairs ready
    sem_init(&mutex, 0, 1);              // Binary lock

    // Create Producer and Consumer Threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&prod_threads[i], NULL, producer, &prod_ids[i]);
    }
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Join threads (in a real app, you'd manage shutdown)
    pthread_join(cons_thread, NULL);

    return 0;
}