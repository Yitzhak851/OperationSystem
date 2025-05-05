#ifndef TAS_SEMAPHORE_H
#define TAS_SEMAPHORE_H
#include <stdatomic.h>
typedef struct {
    int value; // Semaphore value
    int lock; // Spinlock for mutual exclusion
} semaphore sem;

void semaphore_init(semaphore* sem, int initial_value);

void semaphore_wait(semaphore* sem);

void semaphore_signal(semaphore* sem);

#endif // TAS_SEMAPHORE_H
