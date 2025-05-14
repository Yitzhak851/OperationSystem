#ifndef TL_SEMAPHORE_H
#define TL_SEMAPHORE_H

#include <stdatomic.h>

typedef struct {
    atomic_int ticket;
    atomic_int cur_ticket;
    atomic_int value;
} semaphore;

void semaphore_init(semaphore* sem, int initial_value);
void semaphore_wait(semaphore* sem);
void semaphore_signal(semaphore* sem);

#endif // TL_SEMAPHORE_H