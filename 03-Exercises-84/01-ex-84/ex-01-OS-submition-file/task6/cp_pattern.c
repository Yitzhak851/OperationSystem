#include "cp_pattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <stdint.h>
#include <pthread.h>
#include "cond_var.h"
#include "rw_lock.c"
#include "helper_functions.h"
#include <unistd.h>

int shared_queue[1000000];
atomic_int number_of_produced_numbers;
atomic_int number_of_checked_numbers;
int consumers, producers;
int can_consume;
int is_produced_array[1000000];
int is_checked_array[1000000];
int current_number_to_check;
pthread_t *producers_threads, *consumers_threads;
condition_variable is_empty;
ticket_lock external_lock;
ticket_lock printer_lock;
rwlock readers_writer_lock;
atomic_bool is_printing;

/* 
 *  TODO: Implement start_consumers_producers. This function should:
 *  - Print the configuration (number of consumers, producers, seed).
 *  - Seed the random number generator using srand().
 *  - Create producer and consumer threads.
 */
void start_consumers_producers(int consumers, int producers, int seed) {
    // TODO: Print configuration and start threads.
    // Prints configuration
    printf("Number of Consumers: %d\n" , consumers);
    printf("Number of Producers: %d\n", producers);
    printf("Seed: %d\n", seed);
    srand(seed); // Seeds the random number generator
    atomic_init(&number_of_produced_numbers, 0);
    atomic_init(&number_of_checked_numbers, 0);
    producers_threads = malloc(sizeof(pthread_t) * producers); // Allocates the producers_threads array
    consumers_threads = malloc(sizeof(pthread_t) * consumers); // Allocates the consumers_threads array
    condition_variable_init(&is_empty);
    can_consume = 0;
    // Creates the producers threads
    for (int i = 0; i < producers; i++) {
        pthread_create(&producers_threads[i], NULL, producer_thread_function, NULL);
    }
    // Creates the consumers threads 
    for (int i = 0; i < consumers; i++) {
        pthread_create(&consumers_threads[i], NULL, consumer_thread_function, NULL);
    }
}

/* TODO: Implement stop_consumers to stop all consumers threads. */
void stop_consumers() {
    // TODO: Stop the consumer thread with the given id.
    free(producers_threads); // Frees the producers_threads array
    free(consumers_threads); // Frees the consumers_threads array
}

/* TODO: Implement print_msg to perform synchronized printing. */
void print_msg(const char* msg) {
    // TODO: Print the message ensuring output does not overlap.
    printf(msg, pthread_self(), current_number_to_check, divides_by_six(current_number_to_check));

}

/* TODO: Implement wait_until_producers_produced_all_numbers The function should wait until all numbers between 0 and 1,000,000 have been produced. */
void wait_until_producers_produced_all_numbers() {
    // TODO: Wait until production of numbers (0 to 1,000,000) is complete.
    // Producing all numbers between 0 and 1,000,000
    for (int i = 0; i < producers; i++) {
        pthread_join(producers_threads[i], NULL);
    }
}

/* TODO: Implement wait_consumers_queue_empty to wait until queue is empty, if queue is already empty - return immediately without waiting. */
void wait_consumers_queue_empty() {
    // TODO: Return non-zero if the consumer queue is empty.
    if (!is_queue_empty()) { // Checks if consumer queue is empty
        for (int i = 0; i < consumers; i++) { // Runs the counsumers threads
            pthread_join(consumers_threads[i], NULL);
        }
    }
    return;
}

void* producer_thread_function(void* data) {
    while (!can_consume) {
        rwlock_acquire_write(&readers_writer_lock); // Acquires the readers-writer lock
        int next_produced_number = rand() % 1000000; // Generates random numbers in range 0 to 1,000,000
        if (!is_produced_array[next_produced_number]) { // Checks if number is already produced
            is_produced_array[next_produced_number] = 1;
            fprintf(stdout, "Producer %ld generated number: %d\n", pthread_self(), next_produced_number); 
            shared_queue[atomic_load(&number_of_produced_numbers)] = next_produced_number; // After printing, pushes the number to a shared queue
            atomic_fetch_add(&number_of_produced_numbers, 1);
            // Once all numbers have been generated, a producer should signal a global condition variable
            if (atomic_load(&number_of_produced_numbers) == 1000000) {
                can_consume = 1;
                condition_variable_broadcast(&is_empty);
            }
        }
        rwlock_release_write(&readers_writer_lock); // Releases the readers-writer lock
    }
    return NULL;
}

void* consumer_thread_function(void* data) {
    while(atomic_load(&number_of_checked_numbers) != 1000000) {
        ticketlock_acquire(&external_lock);
        // Waits until producers have done their work
        while(!can_consume) {
            condition_variable_wait(&is_empty, &external_lock);
        }
        current_number_to_check = shared_queue[atomic_load(&number_of_checked_numbers)];
        ticketlock_release(&external_lock);
        ticketlock_acquire(&printer_lock); // Acquiers the printer lock
        if (!is_checked_array[current_number_to_check]) { 
            is_checked_array[current_number_to_check] = 1;
            atomic_fetch_add(&number_of_checked_numbers, 1);
            char* msg = "Consumer %ld checked %d. Is it divisible by 6? %s.\n"; // Creates a formatted message
            print_msg(msg);
        }
        can_consume = atomic_load(&number_of_checked_numbers) != 1000000;
        ticketlock_release(&printer_lock); // Releases the printer lock
    }
    return NULL;  
}

// Checks if number divides by 6
const char* divides_by_six(int number) {
    char* answer;
    if (number % 6 == 0) {
        answer = "True";
    } else {
        answer = "False";
    }
    return answer;
}

// Checks if the shared queue is empty
int is_queue_empty() {
    for (int i = 0; i < 1000000; i++) {
        if (is_checked_array[i]) {
            return 0;
        }
    }
    return 1;
}

/* TODO: Implement a main function that controls the producer-consumer process */
int main(int argc, char* argv[]) {
    /* TODO: Parse arguments , Start producer-consumer process, Wait for threads to finish and clean up resources. Checks that the argument count is correct. */
    if (argc != 4) {
        printf("usage: cp_pattern [consumers] [producers] [seed]\n");
        return 1;
    }
    int input_array[4];
    // Checks that the argument values are correct, then parses them
    for (int i = 1; i <= 3; i++) {
        for (int j = 0; j < strlen(argv[i]); j++) {
            if (argv[i][j] < '0' || argv[i][j] > '9') {
                printf("usage: cp_pattern [consumers] [producers] [seed]\n");
                return 1;
            } else {
                input_array[i] = input_array[i] * 10 + (argv[i][j] - '0');
            }
        }
    }
    consumers = input_array[1];
    producers = input_array[2];
    int seed = input_array[3];
    if (producers <= 0 || consumers <= 0) {
        printf("usage: cp_pattern [consumers] [producers] [seed]\n");
        return 1;
    }
    ticketlock_init(&external_lock);
    ticketlock_init(&printer_lock);
    // Inits the shared queue
    for (int i = 0; i < 1000000; i++) {
        rwlock_acquire_write(&readers_writer_lock);
        shared_queue[i] = -1;
        rwlock_release_write(&readers_writer_lock);
    }
    for (int i = 0; i < 1000000; i++) {
        rwlock_acquire_write(&readers_writer_lock);
        is_produced_array[i] = 0;
        rwlock_release_write(&readers_writer_lock);
    }
    for (int i = 0; i < 1000000; i++) {
        rwlock_acquire_write(&readers_writer_lock);
        is_checked_array[i] = 0;
        rwlock_release_write(&readers_writer_lock);
    }
    start_consumers_producers(consumers, producers, seed);
    wait_until_producers_produced_all_numbers();
    wait_consumers_queue_empty();
    stop_consumers();
    return 0;
}