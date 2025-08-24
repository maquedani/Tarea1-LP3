#include <windows.h>
#include <stdio.h>

/* The TLS index used to associate a log file pointer with each thread. */
static DWORD thread_log_key;

/* Write MESSAGE to the log file for the current thread. */
void write_to_thread_log(const char* message)
{
    FILE* thread_log = (FILE*)TlsGetValue(thread_log_key);
    if (thread_log != NULL)
        fprintf(thread_log, "%s\n", message);
}

/* Close the log file pointer THREAD_LOG. */
void close_thread_log()
{
    FILE* thread_log = (FILE*)TlsGetValue(thread_log_key);
    if (thread_log != NULL)
    {
        fclose(thread_log);
        TlsSetValue(thread_log_key, NULL);
    }
}

DWORD WINAPI thread_function(LPVOID args)
{
    char thread_log_filename[20];
    FILE* thread_log;
    DWORD thread_id = GetCurrentThreadId();

    /* Generate the filename for this thread's log file. */
    sprintf(thread_log_filename, "thread%lu.log", (unsigned long)thread_id);

    /* Open the log file. */
    thread_log = fopen(thread_log_filename, "w");

    /* Store the file pointer in thread-specific data under thread_log_key. */
    TlsSetValue(thread_log_key, thread_log);

    write_to_thread_log("Thread starting.");

    /* Do work here... */

    close_thread_log();
    return 0;
}

int main()
{
    int i;
    HANDLE threads[5];

    /* Allocate a TLS index to associate thread log file pointers. */
    thread_log_key = TlsAlloc();
    if (thread_log_key == TLS_OUT_OF_INDEXES)
    {
        printf("Failed to allocate TLS index.\n");
        return 1;
    }

    /* Create threads to do the work. */
    for (i = 0; i < 5; ++i)
    {
        threads[i] = CreateThread(
            NULL,
            0,
            thread_function,
            NULL,
            0,
            NULL
        );

        if (threads[i] == NULL)
        {
            printf("Failed to create thread %d.\n", i);
            return 1;
        }
    }

    /* Wait for all threads to finish. */
    WaitForMultipleObjects(5, threads, TRUE, INFINITE);

    /* Close thread handles. */
    for (i = 0; i < 5; ++i)
        CloseHandle(threads[i]);

    /* Free TLS index. */
    TlsFree(thread_log_key);

    return 0;
}
