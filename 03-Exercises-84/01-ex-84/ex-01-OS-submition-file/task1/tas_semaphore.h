#ifndef TAS_SEMAPHORE_H
#define TAS_SEMAPHORE_H

#include <stdatomic.h> // This header file provides atomic operations.

/*
 * Define the semaphore type.
 * Write your struct details in this file..
 */
typedef struct {
    // write your implementation here
    atomic_int value; // Semaphore's value
    atomic_flag lock; // Semaphore's lock
} semaphore;


/*
 * Initializes the semaphore pointed to by 'sem' with the specified initial value.
 */
void semaphore_init(semaphore* sem, int initial_value);

/*
 * Decrements the semaphore (wait operation).
 */
void semaphore_wait(semaphore* sem);

/*
 * Increments the semaphore (signal operation).
 */
void semaphore_signal(semaphore* sem);

#endif // TAS_SEMAPHORE_H
