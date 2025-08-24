#include <windows.h>
#include <stdio.h>

/* Compute successive prime numbers (very inefficiently). Return the
Nth prime number, where N is the value pointed to by ARG. */
DWORD WINAPI compute_prime(LPVOID arg)
{
    int candidate = 2;
    int n = *((int*) arg);

    while (1) {
        int factor;
        int is_prime = 1;

        /* Test primality by successive division. */
        for (factor = 2; factor < candidate; ++factor)
            if (candidate % factor == 0) {
                is_prime = 0;
                break;
            }

        /* Is this the prime number we're looking for? */
        if (is_prime) {
            if (--n == 0)
                /* Return the desired prime number as the thread return value. */
                return (DWORD_PTR) candidate;
        }
        ++candidate;
    }

    return 0;
}

/* The main program. */
int main()
{
    HANDLE thread;
    DWORD thread_result;
    int which_prime = 5000;
    int prime;

    /* Start the computing thread, up to the 5,000th prime number. */
    thread = CreateThread(
        NULL,           /* default security attributes */
        0,              /* default stack size */
        compute_prime,  /* thread function */
        &which_prime,   /* parameter to thread function */
        0,              /* default creation flags */
        NULL            /* thread identifier */
    );

    if (thread == NULL) {
        printf("Failed to create thread.\n");
        return 1;
    }

    /* Do some other work here... */

    /* Wait for the prime number thread to complete, and get the result. */
    WaitForSingleObject(thread, INFINITE);
    GetExitCodeThread(thread, &thread_result);
    prime = (int) thread_result;

    /* Print the largest prime it computed. */
    printf("The %dth prime number is %d.\n", which_prime, prime);

    CloseHandle(thread);
    return 0;
}
