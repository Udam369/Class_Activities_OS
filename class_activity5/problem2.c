#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Declare semaphores
sem_t a;
sem_t b;
sem_t c;

// Process 1: Prints 'H' and 'E', then signals Process 2 twice
void* process1(void* arg) {
    while (1) {
        sem_wait(&a);         // Wait for permission to start (initial value 1)
        
        printf("H");
        fflush(stdout);       // Force immediate output to console
        
        printf("E");
        fflush(stdout);
        
        sem_post(&b);         // First signal to allow one 'L'
        sem_post(&b);         // Second signal to allow a second 'L'
        
        // Loops back, calls sem_wait(&a). 'a' is now 0, so Thread 1 blocks forever.
    }
    return NULL;
}

// Process 2: Prints 'L', then signals Process 3
void* process2(void* arg) {
    while (1) {
        sem_wait(&b);         // Wait for signal from Process 1
        
        printf("L");
        fflush(stdout);
        
        sem_post(&c);         // Signal Process 3 that an 'L' was printed
        
        // Loops back. After 2 iterations, 'b' is 0, so Thread 2 blocks forever.
    }
    return NULL;
}

// Process 3: Prints 'O'
void* process3(void* arg) {
    while (1) {
        sem_wait(&c);         // Wait for first 'L'
        sem_wait(&c);         // Wait for second 'L'
        
        printf("O\n");        // Print 'O' and a newline
        fflush(stdout);
        
        // Loops back. 'c' is now 0, so Thread 3 blocks forever.
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    // Initialize semaphores
    // sem_init(pointer, shared_between_processes_flag, initial_value)
    sem_init(&a, 0, 1); 
    sem_init(&b, 0, 0); 
    sem_init(&c, 0, 0); 

    // Create threads
    pthread_create(&t1, NULL, process1, NULL);
    pthread_create(&t2, NULL, process2, NULL);
    pthread_create(&t3, NULL, process3, NULL);

    // Join threads to prevent main from exiting
    // Note: Because the threads intentionally block after printing "HELLO",
    // these joins will wait indefinitely. You will need to terminate the 
    // process manually (e.g., Ctrl+C) after execution.
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // Destroy semaphores (this will not be reached in this specific execution model)
    sem_destroy(&a);
    sem_destroy(&b);
    sem_destroy(&c);

    return 0;
}