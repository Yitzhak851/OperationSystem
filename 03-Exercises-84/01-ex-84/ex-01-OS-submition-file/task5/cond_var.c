// ticket_lock.c
#include <stdatomic.h> // This header file provides atomic operations.
#include <sched.h> // This header file provides the sched_yield function for yielding the CPU.
#include <pthread.h> // This header file provides the pthread_yield function for yielding the CPU.

#include "cond_var.h" // This is the header file for the condition variable implementation.
#include "ticket_lock.h" // This is the header file for the Ticket Lock implementation.

/*
 * Initializes the condition variable.
 * @param cv Pointer to the condition variable structure.
 */
void condition_variable_init(condition_variable *cv) {
    atomic_init(&cv->waiter_count, 0);
    atomic_init(&cv->number_of_released_threads, 0);
}

/*
 * Waits on the condition variable.
 * This function increases the waiter count, releases the external lock, and waits until signaled.
 * @param cv Pointer to the condition variable structure.
 * @param ext_lock Pointer to the external lock (ticket lock) to be released while waiting.
 */
void condition_variable_wait(condition_variable *cv, ticket_lock *ext_lock) {
    int counter_val = atomic_fetch_add(&cv->waiter_count, 1) ; // Increases waiter count
    while (atomic_load(&cv->number_of_released_threads) <= counter_val) {
        ticketlock_release(ext_lock); // Releases ext_lock
        sched_yield(); // Waits until signaled
        ticketlock_acquire(ext_lock); // Reacquire ext_lock 
    }
}

/* 
 * Signals one waiting thread.
 * This function increments the number of released threads, allowing one waiting thread to proceed.
 * @param cv Pointer to the condition variable structure.
 */
void condition_variable_signal(condition_variable *cv) {
    atomic_fetch_add(&cv->number_of_released_threads, 1); // Increments the number of released threads
}

/*
 * Signals all waiting threads.
 * This function sets the number of released threads to the number of waiting threads.
 * @param cv Pointer to the condition variable structure.
 */
void condition_variable_broadcast(condition_variable *cv){
    atomic_store(&cv->number_of_released_threads, atomic_load(&cv->waiter_count));
}

/*
 * Initializes the ticket lock.
 * @param lock Pointer to the ticket lock structure.
 * This function initializes the ticket and current ticket values to 0.
 * The ticket lock is used to ensure mutual exclusion in a multi-threaded environment.
 */
void ticketlock_init(ticket_lock* lock){
    atomic_init(&lock->ticket, 0);
    atomic_init(&lock->cur_ticket, 0);
}

/*
 * Acquires the ticket lock.
 * This function uses a busy-wait loop to acquire the lock.
 * @param lock Pointer to the ticket lock structure.
 * The function increments the ticket value and waits until it is the thread's turn to acquire the lock.
 */
void ticketlock_acquire(ticket_lock* lock){
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
 * The function increments the current ticket value, allowing the next thread to proceed.
 */
void ticketlock_release(ticket_lock* lock){
    atomic_fetch_add(&lock->cur_ticket, 1);
}