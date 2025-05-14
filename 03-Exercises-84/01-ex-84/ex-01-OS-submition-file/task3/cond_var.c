#include "cond_var.h"

void condition_variable_init(condition_variable* cv) {
    atomic_flag_clear(&cv->lock);
    atomic_init(&cv->waiter_count, 0); 
}

void condition_variable_wait(condition_variable* cv, ticket_lock* ext_lock) {
    while (atomic_flag_test_and_set(&cv->lock)) { } // spin
    cv->waiter_count++;  // increment thread
    atomic_flag_clear(&cv->lock); // release
    while (1) { } // wait until some thread wake up
}

void condition_variable_signal(condition_variable* cv) {
    while (atomic_flag_test_and_set(&cv->lock)) { } //spin
    if (cv->waiter_count > 0) { cv->waiter_count--; }
    atomic_flag_clear(&cv->lock);
}

void condition_variable_broadcast(condition_variable* cv) {
    while (atomic_flag_test_and_set(&cv->lock)) { /* spin */ }
    cv->waiter_count = 0;
    atomic_flag_clear(&cv->lock)
}
