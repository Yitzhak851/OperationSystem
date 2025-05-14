#include "tl_semaphore.h"
#include <stdatomic.h> // For atomic operations
#include <sched.h>  // For sched_yield
#include <stdio.h>  // For printf (optional, for debugging)

void semaphore_init(semaphore* sem, int initial_value) {
    atomic_init(&sem->ticket, 0);
    atomic_init(&sem->cur_ticket, 0);
    sem->value = initial_value; 
}

void semaphore_wait(semaphore* sem) {
    int my_ticket = atomic_fetch_add(&sem->ticket, 1); // get my ticket
    while (atomic_load(&sem->cur_ticket) != my_ticket){} // wait until is my turn
    while (sem->value <= 0) {} // wait until a resource is availble
    sem->value--;
    atomic_fetch_add(&sem->ticket, 1); // advance to next turn
}

void semaphore_signal(semaphore* sem) {
    atomic_fetch_add(&sem->ticket, 1);
}