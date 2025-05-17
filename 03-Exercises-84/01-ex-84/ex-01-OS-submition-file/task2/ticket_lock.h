// ticket_lock.h

#ifndef TICKET_LOCK_H
#define TICKET_LOCK_H

#include <stdatomic.h>
#include <sched.h>

/* Define the ticket lock type. */
typedef struct {
    atomic_int ticket;
    atomic_int cur_ticket;
} ticket_lock;


void ticketlock_init(ticket_lock* lock); /* Initializes the ticket lock */
void ticketlock_acquire(ticket_lock* lock); /* Acquires the ticket lock */
void ticketlock_release(ticket_lock* lock); /* Releases the ticket lock */

#endif // TICKET_LOCK_H