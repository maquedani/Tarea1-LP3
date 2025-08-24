#include <windows.h>
#include <stdio.h>

/* Thread function. Does some work here... */
DWORD WINAPI thread_function(LPVOID thread_arg)
{
    /* Do work here... */
    printf("Detached thread is running.\n");
    Sleep(2000);  // simulate work
    printf("Detached thread finished.\n");
    return 0;
}

/* The main program. */
int main()
{
    HANDLE thread;
    
    /* Create a new thread in a "detached" manner. */
    thread = CreateThread(
        NULL,           /* default security attributes */
        0,              /* default stack size */
        thread_function,/* thread function */
        NULL,           /* parameter to thread function */
        0,              /* default creation flags */
        NULL            /* thread identifier */
    );

    if (thread == NULL) {
        printf("Failed to create thread.\n");
        return 1;
    }

    /* Destroy handle immediately: thread is now "detached" */
    CloseHandle(thread);

    /* Do work here... in main thread */
    printf("Main thread doing work...\n");
    Sleep(3000);
    printf("Main thread finished.\n");

    /* No need to wait for the detached thread. */
    return 0;
}
