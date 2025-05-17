#include "local_storage.h"
#include "rw_lock.h"

tls_data_t g_tls[MAX_THREADS]; /* TODO: Define the global TLS array. TODO: Set all thread_id fields to -1 and data pointers to NULL. */

/* TODO: Implement init_storage to initialize g_tls. */
void init_storage(void) {
    rwlock lock;
    rwlock_init(&lock); // Inits a readers-writer-lock
    for (int i = 0; i < MAX_THREADS; i++) {
        rwlock_acquire_write(&lock); // Acquires the writer's lock
        g_tls[i].thread_id = -1;
        g_tls[i].data = NULL;
        rwlock_release_write(&lock); // Releases the writer's lock
    }
}

/* TODO: Implement tls_thread_alloc to allocate a TLS entry for the calling thread. TODO: Use your synchronization mechanism to safely allocate an entry. */
void tls_thread_alloc(void) {
    int64_t current_thread_id = pthread_self(); // Gets the thread's ID
    int number_of_checked_slots = 0;
    rwlock lock;
    rwlock_init(&lock); // Inits a readers-writer-lock
    rwlock_acquire_read(&lock); // Acquires the readers' lock
    for(; number_of_checked_slots < MAX_THREADS; number_of_checked_slots++) {
        if (g_tls[number_of_checked_slots].thread_id == current_thread_id) {
            rwlock_release_read(&lock); // Releases the readers' lock
            return;
        } 
        if (g_tls[number_of_checked_slots].thread_id == -1) {
            rwlock_release_read(&lock); // Releases the readers' lock
            rwlock_acquire_write(&lock); // Acquires the writer's lock
            g_tls[number_of_checked_slots].thread_id = current_thread_id;
            rwlock_release_write(&lock); // Releases the writer's lock
            return;
        }
    }
    // thread local storage is full
    rwlock_release_read(&lock); // Releases the readers' lock
    printf("thread %ld failed to initialize, not enough space\n", current_thread_id);
    exit(1);
}

/* TODO: Implement get_tls_data to retrieve the TLS data for the calling thread. TODO: Search for the calling thread's entry and return its data. */
void* get_tls_data(void) {
    int64_t current_thread_id = pthread_self(); // Gets the thread's ID
    int number_of_checked_slots = 0;
    rwlock lock;
    rwlock_init(&lock); // Inits a readers-writer-lock
    rwlock_acquire_read(&lock); // Acquires the readers' lock
    for(; number_of_checked_slots < MAX_THREADS; number_of_checked_slots++) {
        if (g_tls[number_of_checked_slots].thread_id == current_thread_id) { // Finds the relevant entry in the TLS
            void* tls_data = g_tls[number_of_checked_slots].data; // Retrieves the data from TLS
            rwlock_release_read(&lock); // Releases the readers' lock
            return tls_data; // Returns the data
        }
    }
    // thread’s entry is not found
    rwlock_release_read(&lock); // Releases the readers' lock
    printf("thread %ld hasn’t been initialized in the TLS\n", current_thread_id);
    exit(2);
}

/* TODO: Implement set_tls_data to update the TLS data for the calling thread. TODO: Search for the calling thread's entry and set its data. */
void set_tls_data(void* data) {
    int64_t current_thread_id = pthread_self(); // Gets the thread's ID
    int number_of_checked_slots = 0;
    rwlock lock;
    rwlock_init(&lock); // Inits a readers-writer-lock
    rwlock_acquire_read(&lock); // Acquires the readers' lock
    for(; number_of_checked_slots < MAX_THREADS; number_of_checked_slots++) {
        if (g_tls[number_of_checked_slots].thread_id == current_thread_id) {
            rwlock_release_read(&lock); // Releases the readers' lock
            rwlock_acquire_write(&lock); // Acquires the writer's lock
            g_tls[number_of_checked_slots].data = data; // Sets the data in the tls
            rwlock_release_write(&lock); // Releases the writer's lock
            return;
        }
    }
    // thread’s entry is not found
    rwlock_release_read(&lock); // Releases the readers' lock
    printf("thread %ld hasn’t been initialized in the TLS\n", current_thread_id);
    exit(2);
}

/* TODO: Implement tls_thread_free to free the TLS entry for the calling thread.  TODO: Reset the thread_id and data in the corresponding TLS entry.*/
void tls_thread_free(void) {
    int64_t current_thread_id = pthread_self(); // Gets the thread's ID
    int number_of_checked_slots = 0;
    rwlock lock;
    rwlock_init(&lock); // Inits a readers-writer-lock
    rwlock_acquire_read(&lock); // Acquires the readers' lock
    for(; number_of_checked_slots < MAX_THREADS; number_of_checked_slots++) {
        if (g_tls[number_of_checked_slots].thread_id == current_thread_id) {
            rwlock_release_read(&lock); // Releases the readers' lock
            rwlock_acquire_write(&lock); // Acquires the writer's lock
            g_tls[number_of_checked_slots].thread_id = -1;
            g_tls[number_of_checked_slots].data = NULL;
            rwlock_release_write(&lock); // Releases the writer's lock
            return;
        }
    }
    rwlock_release_read(&lock); // Releases the readers' lock
}