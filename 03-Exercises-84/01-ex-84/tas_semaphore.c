#include "tas_semaphore.h"

/*
 * TODO: Implement semaphore_init using a TAS spinlock.
 */
void semaphore_init(semaphore* sem, int initial_value) {
    // TODO: Define the structure and initialize the semaphore.
}

/*
 * TODO: Implement semaphore_wait using the TAS spinlock mechanism.
 */
void semaphore_wait(semaphore* sem) {
    // TODO: Acquire the spinlock, decrement the semaphore value, then release the spinlock.
}

/*
 * TODO: Implement semaphore_signal using the TAS spinlock mechanism.
 */
void semaphore_signal(semaphore* sem) {
    // TODO: Acquire the spinlock, increment the semaphore value, then release the spinlock.
}
