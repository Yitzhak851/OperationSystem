#ifndef COND_VAR_H
#define COND_VAR_H
#include <stdatomic.h>
#include <stdbool.h>

typedef struct {
    atomic_flag lock;
    atomic_int waiter_count;
} condition_variable;

typedef struct {
    atomic_int next_ticket;
    atomic_int now_serving;
} ticket_lock; // Simple ticket lock for external locking

void condition_variable_init(condition_variable* cv);
void condition_variable_wait(condition_variable* cv, ticket_lock* ext_lock);
void condition_variable_signal(condition_variable* cv);
void condition_variable_broadcast(condition_variable* cv);

#endif // COND_VAR_H