/*Listing 4: Example of using sigaction to handle SIGINT in a general case*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGINT
void signal_handler(int signum)
{
    printf("Received signal %d (SIGINT). Exiting gracefully ...\n", signum);
    exit(0);
}

int main(void)
{
    struct sigaction sa;

    // Set up the sigaction structure to specify the signal handler
    sa.sa_handler = signal_handler; // Set the signal handler function
    sigemptyset(&sa.sa_mask);       // No additional signals blocked during
    execution of the handler
        sa.sa_flags = 0; // No special flags

    // Register the signal handler for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Program is running. Press Ctrl+C to trigger SIGINT .\n");

    // Main loop: the program will continue running until it receives SIGINT
    while (1)
    {
        sleep(1);
    }

    return 0;
}