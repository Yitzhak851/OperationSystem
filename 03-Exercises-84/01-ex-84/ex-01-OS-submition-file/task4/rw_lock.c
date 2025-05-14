#include "rw_lock.h"

void rwlock_init(rwlock* lock) {
    atomic_flag_clear(&lock->lock);
    atomic_init(&lock->readers, 0);
}

void rwlock_acquire_read(rwlock* lock) {
    // if now we at write lock, we need to wait TODO: add mutex
    if (atomic_flag_test_and_set(&lock->lock)) {
        while (atomic_flag_test_and_set(&lock->lock)) { /* spin */ }
    }
    // if we are not in write lock, we can increase the reader count   
    // and release the lock
    while (atomic_flag_test_and_set(&lock->lock)) { /* spin */ }
    lock->readers++;
    atomic_flag_clear(&lock->lock);
}

void rwlock_release_read(rwlock* lock) {
    while (atomic_flag_test_and_set(&lock->lock)) { /* spin */ }
    lock->readers--;
    atomic_flag_clear(&lock->lock);
}

void rwlock_acquire_write(rwlock* lock) {
    while (atomic_flag_test_and_set(&lock->lock)) { /* spin */ }
    while (lock->readers > 0) { /*TODO - add mutex */}
    atomic_flag_clear(&lock->lock);
}

void rwlock_release_write(rwlock* lock) {
    // TODO: Release the write lock.
}