#include "cp_pattern.h" 

void* producer_thread_function(void* data); /* Producer thread function */
int is_produced(int number); /* Checks if a number is already produced */
void* consumer_thread_function(void* data); /* Consumer thread function */
const char* divides_by_six(int number); /* Checks if number divides by 6 */
int is_queue_empty(); /* Checks if the shared queue is empty */