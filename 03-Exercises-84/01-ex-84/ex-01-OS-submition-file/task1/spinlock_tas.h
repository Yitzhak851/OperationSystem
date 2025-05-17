// spinlock.h
#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdatomic.h> // For atomic operations
#include <sched.h> // For sched_yield
#include <pthread.h> // For pthread_yield

typedef struct {
    atomic_flag lock;
} spinlock;

void spinlock_init(spinlock* lock);
void spinlock_acquire(spinlock* lock);
void spinlock_release(spinlock* lock);

#endif // SPINLOCK_H