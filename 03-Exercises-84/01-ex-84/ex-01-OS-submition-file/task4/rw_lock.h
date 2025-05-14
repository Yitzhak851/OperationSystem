#ifndef RW_LOCK_H
#define RW_LOCK_H
#include <stdatomic.h>

typedef struct {
    atomic_flag lock;
    atomic_int readers;
} rwlock;

void rwlock_init(rwlock* lock);
void rwlock_acquire_read(rwlock* lock);
void rwlock_release_read(rwlock* lock);
void rwlock_acquire_write(rwlock* lock);
void rwlock_release_write(rwlock* lock);

#endif // RW_LOCK_H