#ifndef RW_LOCK_H
#define RW_LOCK_H
#include <stdatomic.h>

/*
 * Define the read-write lock type.
 * Write your struct details in this file..
 */
typedef struct {
    // write your implementation here
    atomic_int reader_count;
    atomic_bool is_writing;
    ticket_lock tickets_lock;
    condition_variable can_access;
} rwlock;

void rwlock_init(rwlock* lock);
void rwlock_acquire_read(rwlock* lock);
void rwlock_release_read(rwlock* lock);
void rwlock_acquire_write(rwlock* lock);
void rwlock_release_write(rwlock* lock);

#endif // RW_LOCK_H