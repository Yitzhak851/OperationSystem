// ticket_lock.c
#include <stdatomic.h> // This header file provides atomic operations.
#include <sched.h> // This header file provides the sched_yield function for yielding the CPU.
#include <pthread.h> // This header file provides the pthread_yield function for yielding the CPU.

#include "ticket_lock.h" // This is the header file for the ticket lock implementation.
#include "tl_semaphore.h" // This is the header file for the Ticket Lock semaphore implementation.

/* 
 * Initializes the ticket lock.
 * @param lock Pointer to the ticket lock structure.
 */
void ticketlock_init(ticket_lock* lock) {
    atomic_init(&lock->ticket, 0);
    atomic_init(&lock->cur_ticket, 0);
}

/*
 * Acquires the ticket lock.
 * This function uses a busy-wait loop to acquire the lock.
 * @param lock Pointer to the ticket lock structure.
 */
void ticketlock_acquire(ticket_lock* lock) {
    int my_ticket = atomic_fetch_add(&lock->ticket, 1); // get my ticket
    // wait until it is my turn
    while (atomic_load(&lock->cur_ticket) != my_ticket) {
        sched_yield();
    }
}

/*
 * Releases the ticket lock.
 * This function increments the current ticket, allowing the next thread to acquire the lock.
 * @param lock Pointer to the ticket lock structure.
 */
void ticketlock_release(ticket_lock* lock) {
    atomic_fetch_add(&lock->cur_ticket, 1);
}