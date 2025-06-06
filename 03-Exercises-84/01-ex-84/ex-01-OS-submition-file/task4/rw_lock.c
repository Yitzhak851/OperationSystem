// ticket_lock.c
#include <stdatomic.h> // This header file provides atomic operations.
#include <sched.h> // This header file provides the sched_yield function for yielding the CPU.
#include <pthread.h> // This header file provides the pthread_yield function for yielding the CPU.

#include "rw_lock.h"
#include "cond_var.h"

/* 
 * Initializes the read-write lock.
 * @param lock Pointer to the read-write lock structure.
 */
void rwlock_init(rwlock* lock) {
    atomic_init(&lock->reader_count, 0);
    atomic_init(&lock->is_writing, 0);
    ticketlock_init(&lock->tickets_lock);
    condition_variable_init(&lock->can_access);
}

/* 
 * Acquires the lock for reading.
 * This function allows multiple readers to access the resource while ensuring that no writer is active.
 * @param lock Pointer to the read-write lock structure.
 */
void rwlock_acquire_read(rwlock* lock) {
    ticketlock_acquire(&lock->tickets_lock); // Tries to get the lock for start reading
    while (atomic_load(&lock->is_writing)) { // ensuring no writer is active.
        condition_variable_wait(&lock->can_access, &lock->tickets_lock); // Waits till the writer finishes his work
    }
    atomic_fetch_add(&lock->reader_count, 1); // Increases the reader counter
    ticketlock_release(&lock->tickets_lock);
}

/* 
 * Releases the lock after reading.
 * This function decrements the reader count and signals any waiting writers.
 * @param lock Pointer to the read-write lock structure.
 */
void rwlock_release_read(rwlock* lock) {
    ticketlock_acquire(&lock->tickets_lock); // Tries to get the lock
    atomic_fetch_sub(&lock->reader_count, 1); // Decreases the reader counter
    ticketlock_release(&lock->tickets_lock);
}

/* 
 * Acquires the lock for writing.
 * This function ensures exclusive access to the resource by blocking both readers and other writers.
 * @param lock Pointer to the read-write lock structure.
 */
void rwlock_acquire_write(rwlock* lock) {
    ticketlock_acquire(&lock->tickets_lock); // Tries to get the lock
    while (atomic_load(&lock->reader_count) || atomic_load(&lock->is_writing)) {
        condition_variable_wait(&lock->can_access, &lock->tickets_lock); // Waits till the writer finishes his work
    }
    atomic_store(&lock->is_writing, 1); // Begins writing
}

/**
 * Releases the lock after writing.
 * This function resets the writing flag and signals any waiting readers or writers.
 * @param lock Pointer to the read-write lock structure.
 */
void rwlock_release_write(rwlock* lock) {
    atomic_store(&lock->is_writing, 0); // Ends writing
    ticketlock_release(&lock->tickets_lock); // Re
}