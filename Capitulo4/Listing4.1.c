#include <windows.h>
#include <stdio.h>

/* Prints x's to stderr. The parameter is unused. Does not return. */
DWORD WINAPI print_xs(LPVOID unused)
{
    while (1)
        fputc('x', stderr);
    return 0;
}

/* The main program. */
int main()
{
    HANDLE thread_id;

    /* Create a new thread. The new thread will run the print_xs function. */
    thread_id = CreateThread(
        NULL,       /* default security attributes */
        0,          /* default stack size */
        print_xs,   /* thread function */
        NULL,       /* parameter to thread function */
        0,          /* default creation flags */
        NULL        /* thread identifier */
    );

    if (thread_id == NULL) {
        printf("Failed to create thread.\n");
        return 1;
    }

    /* Print o's continuously to stderr. */
    while (1)
        fputc('o', stderr);

    /* Close the handle to the thread (never reached in this infinite loop). */
    CloseHandle(thread_id);

    return 0;
}
