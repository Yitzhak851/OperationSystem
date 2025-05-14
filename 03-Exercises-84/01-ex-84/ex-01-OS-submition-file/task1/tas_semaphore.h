#ifndef TAS_SEMAPHORE_H
#define TAS_SEMAPHORE_H

#include <stdatomic.h>
#include <sched.h>
#include <stdio.h>

typedef struct {
    atomic_int lock;
    atomic_int value;
} semaphore ;

void semaphore_init(semaphore* sem, int initial_value);
void semaphore_wait(semaphore* sem);
void semaphore_signal(semaphore* sem);

// more_fofo() 
void spinlock_init(atomic_flag* lock);
void spinlock_aquire(atomic_flag* lock);
void spinlock_release(atomic_flag* lock);

#endif // TAS_SEMAPHORE_H