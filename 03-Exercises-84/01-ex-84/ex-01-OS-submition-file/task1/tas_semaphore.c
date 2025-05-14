#include "tas_semaphore.h"
#include <stdatomic.h>
#include <sched.h>
#include <stdio.h>

void semaphore_init(semaphore* sem, int initial_value) {
    atomic_init(&sem->lock, 0); 
    atomic_init(&sem->value, initial_value);
}

void semaphore_wait(semaphore* sem) {
    while(1){
        spinlock_aquire(&sem->lock);
        if (sem->value > 0) {
            sem->value--;
            spinlock_release(&sem->lock);
            break;
        }
        spinlock_release(&sem->lock); // Busy wait
    }
}

void semaphore_signal(semaphore* sem) {
    spinlock_aquire(&sem->lock);
    if (sem->value < o ) {
        sem->value++;
    }
    spinlock_release(&sem->lock);
}

void spinlock_init(atomic_flag* lock){
    atomic_flag_clear(lock);
}

void spinlock_aquire(atomic_flag* lock){
    while(atomic_flag_test_and_set(lock)){
        sched_yield();
    }
}

void spinlock_release(atomic_flag* lock){
    atomic_flag_clear(lock);
}