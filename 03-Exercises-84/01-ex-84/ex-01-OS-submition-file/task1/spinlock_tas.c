#include <stdatomic.h> // This header file provides atomic operations.
#include <sched.h> // This header file provides the sched_yield function for yielding the CPU.
#include <pthread.h> // This header file provides the pthread_yield function for yielding the CPU.

#include "spinlock_tas.h" // This is the header file for the spinlock implementation using Test-And-Set (TAS) mechanism.
#include "tas_semaphore.h" // This is the header file for the TAS semaphore implementation.

/* 
 * Initializes the spinlock.
 * @param lock Pointer to the spinlock structure.
 */
void spinlock_init(atomic_flag* lock) {
    atomic_flag_clear(lock);
}

/*
 * Acquires the spinlock.
 * This function uses a busy-wait loop to acquire the lock.
 * @param lock Pointer to the spinlock structure.
 */
void spinlock_acquire(atomic_flag* lock) {
    while(atomic_flag_test_and_set(lock)) {
        sched_yield(); // scheduler - yield
    }
}

/*
 * Releases the spinlock.
 * This function clears the lock, allowing other threads to acquire it.
 * @param lock Pointer to the spinlock structure.
 */
void spinlock_release(atomic_flag* lock) {
    atomic_flag_clear(lock);
}