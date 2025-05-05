
#include "tas_semaphore.h"

void semaphore_init(semaphore* sem, int initial_value) {
    sem->value = initial_value;
    sem->lock = 0; 
}

void semaphore_wait(semaphore* sem) {

    }

void semaphore_signal(semaphore* sem) {
    // TODO: Acquire the spinlock, increment the semaphore value, then release the spinlock.


}
