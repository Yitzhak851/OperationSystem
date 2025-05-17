#include "rw_lock.h"
#include "cond_var.h"
#include "local_storage.h"


void rwlock_init(rwlock* lock) {
    atomic_init(&lock->reader_count, 0);
    atomic_init(&lock->is_writing, 0);
    ticketlock_init(&lock->tickets_lock);
    condition_variable_init(&lock->can_access);
}

/* TODO: Implement rwlock_acquire_read. TODO: Allow multiple readers while ensuring no writer is active. */
void rwlock_acquire_read(rwlock* lock) {
    ticketlock_acquire(&lock->tickets_lock); // Tries to get the lock for start reading
    while (atomic_load(&lock->is_writing)) { // ensuring no writer is active.
        condition_variable_wait(&lock->can_access, &lock->tickets_lock); // Waits till the writer finishes his work
    }
    atomic_fetch_add(&lock->reader_count, 1); // Increases the reader counter
    ticketlock_release(&lock->tickets_lock);
}

/* TODO: Implement rwlock_release_read. TODO: Decrement the reader count. */
void rwlock_release_read(rwlock* lock) {
    ticketlock_acquire(&lock->tickets_lock); // Tries to get the lock
    atomic_fetch_sub(&lock->reader_count, 1); // Decreases the reader counter
    ticketlock_release(&lock->tickets_lock);
}

/* TODO: Implement rwlock_acquire_write. TODO: Ensure exclusive access for writing. */
void rwlock_acquire_write(rwlock* lock) {
    ticketlock_acquire(&lock->tickets_lock); // Tries to get the lock
    while (atomic_load(&lock->reader_count) || atomic_load(&lock->is_writing)) {
        condition_variable_wait(&lock->can_access, &lock->tickets_lock); // Waits till the writer finishes his work
    }
    atomic_store(&lock->is_writing, 1); // Begins writing
}

/* TODO: Implement rwlock_release_write. TODO: Release the write lock. */
void rwlock_release_write(rwlock* lock) {
    atomic_store(&lock->is_writing, 0); // Ends writing
    ticketlock_release(&lock->tickets_lock); // Re
}