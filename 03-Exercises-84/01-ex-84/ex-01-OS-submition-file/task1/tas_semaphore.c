
#include "tas_semaphore.h"
#include <sched.h>  // עבור sched_yield()

// פונקציית עזר - TAS spinlock
static void tas_lock(atomic_bool* lock) {
    while (atomic_exchange(lock, true)) {
        // עסוק בלחכות עד שהנעילה תתפנה
        sched_yield();  // שחרור ל־CPU כדי לא לשרוף זמן מעבד
    }
}

static void tas_unlock(atomic_bool* lock) {
    atomic_store(lock, false);
}

/*
 * TODO: Implement semaphore_init using a TAS spinlock.
 */
// אתחול הסמפור
void semaphore_init(semaphore* sem, int initial_value) {
    // TODO: Define the structure and initialize the semaphore.
    atomic_init(&sem->value, initial_value);
    atomic_init(&sem->lock, false);
}

/*
 * TODO: Implement semaphore_wait using the TAS spinlock mechanism.
 */

// פעולה של wait (הפחתת הערך)
void semaphore_wait(semaphore* sem) {
    // TODO: Acquire the spinlock, decrement the semaphore value, then release the spinlock.
    while (1) {
        tas_lock(&sem->lock);
        int current = atomic_load(&sem->value);
        if (current > 0) {
            atomic_store(&sem->value, current - 1);
            tas_unlock(&sem->lock);
            break;
        }
        tas_unlock(&sem->lock);
        sched_yield();  // המתנה לפני ניסיון חוזר
    }
}

/*
 * TODO: Implement semaphore_signal using the TAS spinlock mechanism.
 */

// פעולה של signal (הגדלת הערך)
void semaphore_signal(semaphore* sem) {
    // TODO: Acquire the spinlock, increment the semaphore value, then release the spinlock.
    tas_lock(&sem->lock);
    atomic_fetch_add(&sem->value, 1);
    tas_unlock(&sem->lock);
}
