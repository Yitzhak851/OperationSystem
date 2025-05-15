/*Listing 2: Example of using sigaction with a virtual timer (ITIMER VIRTUAL)*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// Signal handler for SIGVTALRM
void timer_handler(int signum) {
    printf("Virtual timer expired\n"); // This function is called each time the virtual timer expires
}

int main(void) {
    struct sigaction sa;
    struct itimerval timer;
    // Set up the SIGVTALRM handler using sigaction
    sa.sa_handler = timer_handler; // Specify our signal handler
    sigemptyset(&sa.sa_mask);      // No signals blocked during the handler
    sa.sa_flags = 0;               // No special flags
    // Register the SIGVTALRM handler
    if (sigaction(SIGVTALRM, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }
    // Configure the virtual timer:
    // The timer will first expire after 1 second , and then every 1 second thereafter.
    timer.it_value.tv_sec = 1;     // Initial expiration in seconds
    timer.it_value.tv_usec = 0;    // Initial expiration in microseconds
    timer.it_interval.tv_sec = 1;  // Subsequent intervals in seconds
    timer.it_interval.tv_usec = 0; // Subsequent intervals in microseconds
    // Start the virtual timer (ITIMER_VIRTUAL counts CPU time used by the process)
    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1)
    {
        perror("setitimer");
        return 1;
    }
    // Busy loop to consume CPU time so that the virtual timer counts down.
    while (1)
    {
        // Perform some CPU -bound task; this loop will trigger timer signals.
        // Note: In a real application , replace this with useful work.
    }
    return 0;
}