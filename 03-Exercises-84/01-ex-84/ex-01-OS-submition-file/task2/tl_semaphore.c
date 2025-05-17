#include <stdatomic.h>
#include <sched.h>

#include "tl_semaphore.h" // This is the header file for the Ticket Lock semaphore implementation.
#include "ticket_lock.h" // This is the header file for the ticket lock implementation.

/*
 * Initializes the semaphore.
 * @param sem Pointer to the semaphore structure.
 * @param initial_value Initial value of the semaphore.
 */
void semaphore_init(semaphore* sem, int initial_value) {
    atomic_init(&sem->value, initial_value); // Initialize the semaphore.
    atomic_init(&sem->ticket, 0); // Inits the ticket lock
    atomic_init(&sem->cur_ticket, 0);
}

/*
 * Waits on the semaphore.
 * This function decrements the semaphore value and blocks the thread if the value is less than or equal to zero.
 * @param sem Pointer to the semaphore structure.
 */
void semaphore_wait(semaphore* sem) {
    while (atomic_load(&sem->value) <= 0) { 
        // Checks if there are free resources
        // Obtains a ticket and waiting until it is our turn
        // get my ticket
        int my_ticket = atomic_fetch_add (&sem->ticket, 1);
        // wait until it is my turn
        while (atomic_load(&sem->cur_ticket) != my_ticket) {
            sched_yield();
        }
    }
    atomic_fetch_sub(&sem->value, 1); // Decreases the semaphore value.
}

/*
 * Signals the semaphore.
 * This function increments the semaphore value and allows the next ticket holder to proceed.
 * It uses the ticket lock mechanism to ensure that the signal operation is atomic and thread-safe.
 * @param sem Pointer to the semaphore structure.
 */
void semaphore_signal(semaphore* sem) {
    atomic_fetch_add(&sem->value, 1); // Increases the semaphore value
    atomic_fetch_add(&sem->cur_ticket, 1);; // Allows the next ticket holder to proceed.
}