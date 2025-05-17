#include <stdatomic.h> // This header file provides atomic operations.
#include <sched.h> // This header file provides the sched_yield function for yielding the CPU.

#include "tas_semaphore.h" // This is the header file for the TAS semaphore implementation.
#include "spinlock_tas.c" // This is the spinlock implementation using Test-And-Set (TAS) mechanism.

/*
 * Initializes the semaphore.
 * @param sem Pointer to the semaphore structure.
 * @param initial_value Initial value of the semaphore.
 */
void semaphore_init(semaphore* sem, int initial_value) {
    atomic_init(&sem->value, initial_value); // Initializes the semaphore.
    spinlock_init(&sem->lock); // Inits the spinlock
}

/*
 * Waits on the semaphore.
 * This function decrements the semaphore value and blocks the thread if the value is less than or equal to zero.
 * @param sem Pointer to the semaphore structure.
 */
void semaphore_wait(semaphore* sem) {
    while (1) {
        spinlock_acquire(&sem->lock); // Acquires the spinlock
        if (atomic_load(&sem->value) > 0) { // Checks if we can enter the critical section
            atomic_fetch_sub(&sem->value, 1); // Decreases the semaphore value
            spinlock_release(&sem->lock); // Releases the spinlock
            break;
        }
        spinlock_release(&sem->lock); // Releases the spinlock
        sched_yield(); // Wating before another try to get the lock
    }
}

/*
 * Increments the semaphore (signal operation).
 * @param sem Pointer to the semaphore structure.
 */
void semaphore_signal(semaphore* sem) {
    spinlock_acquire(&sem->lock); // Acquires the spinlock
    atomic_fetch_add(&sem->value, 1); // Increases the semaphore value
    spinlock_release(&sem->lock); // Releases the spinlock
}