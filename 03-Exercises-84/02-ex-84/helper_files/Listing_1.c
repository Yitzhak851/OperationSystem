/*Listing 1: Simplified cooperative threads demo using sigsetjmp/siglongjmp (64-bit Intel architectures)*/


/*
 * Simplified cooperative threads demo using sigsetjmp/siglongjmp.
 * This example is written for 64-bit Intel architectures only.
 * Two threads are set up with separate stacks , and they switch control
 * by saving and restoring execution contexts.
 */

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

typedef unsigned long address_t; // Define a type for addresses.

// These indices refer to the positions in the jmp_buf where the stack pointer and program counter are stored.
#define JB_SP 6
#define JB_PC 7

// This function adjusts an address for use with sigsetjmp/siglongjmp.
// The inline assembly is used as a black box to "translate" the address.
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor %%fs:0x30 , %0\n"
                 "rol $0x11 , %0\n"
                 : "=g"(ret)
                 : "0"(addr));
    return ret;
}

// Constants for time and stack size.
#define SECOND 1000000  // 1 second in microseconds.
#define STACK_SIZE 4096 // Each thread gets a 4096 - byte stack.

typedef void (*thread_entry_point)(void); // Define a type for thread functions.

// Allocate separate stacks for the two threads.
char stack0[STACK_SIZE];
char stack1[STACK_SIZE];


sigjmp_buf env[2]; // Create jump buffers to store the execution contexts for each thread.
int current_thread = -1; // This variable keeps track of the currently running thread (0 or 1).

// Function to switch to a specific thread using siglongjmp.
void jump_to_thread(int tid)
{
    current_thread = tid;
    siglongjmp(env[tid], 1);
}

// Yield function: save the current thread ’s state and switch to the other thread.
void yield(void)
{
    // Save the current execution context. sigsetjmp returns 0 when saving.
    int ret_val = sigsetjmp(env[current_thread], 1);
    // When coming back from a siglongjmp , sigsetjmp returns a nonzero value.
    if (ret_val == 0)
    {
        // Switch to the other thread (if current is 0, switch to 1; if 1, switch to 0).
        jump_to_thread(1 - current_thread);
    }
    // If ret_val is nonzero , the thread is resuming execution.
}

// The first thread: counts and yields every 3 iterations.
void thread0(void)
{
    int counter = 0;
    while (1)
    {
        counter++;
        printf("Thread 0: counter = %d\n", counter);
        // Yield control after every 3 iterations.
        if (counter % 3 == 0)
        {
            printf("Thread 0 yielding ...\n");
            yield();
        }
        usleep(SECOND); // Sleep for 1 second.
    }
}

// The second thread: counts and yields every 5 iterations.
void thread1(void)
{
    int counter = 0;
    while (1)
    {
        counter++;
        printf("Thread 1: counter = %d\n", counter);
        // Yield control after every 5 iterations.
        if (counter % 5 == 0)
        {
            printf("Thread 1 yielding ...\n");
            yield();
        }
        usleep(SECOND); // Sleep for 1 second.
    }
}

// Set up a thread by initializing its jump buffer to use a new stack and start at a given function.
void setup_thread(int tid, char *stack, thread_entry_point entry_point)
{
    // Calculate the initial stack pointer (start at the top of the stack).
    address_t sp = (address_t)stack + STACK_SIZE - sizeof(address_t);
    // Get the function pointer for the thread ’s starting function.
    address_t pc = (address_t)entry_point;
    // Save the current context into the jump buffer.
    sigsetjmp(env[tid], 1);
    // Manually set the stack pointer and program counter in the jump buffer.
    env[tid]->__jmpbuf[JB_SP] = translate_address(sp);
    env[tid]->__jmpbuf[JB_PC] = translate_address(pc);
    // Clear the saved signal mask.
    sigemptyset(&env[tid]->__saved_mask);
}

// Initialize both threads.
void setup(void)
{
    setup_thread(0, stack0, thread0);
    setup_thread(1, stack1, thread1);
}

// Main entry point: set up threads and start execution.
int main(void)
{
    setup();
    // Begin execution with thread 0.
    jump_to_thread(0);
    return 0;
}