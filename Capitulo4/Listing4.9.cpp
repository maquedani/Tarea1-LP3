#include <pthread.h>
#include <iostream>

/* Exception class used to signal a thread exit with a specific return value. */
class ThreadExitException
{
public:
    /* Create an exception-signaling thread exit with RETURN_VALUE. */
    ThreadExitException(void* return_value)
        : thread_return_value_(return_value)
    {
    }

    /* Actually exit the thread, using the return value provided in the constructor. */
    void* DoThreadExit()
    {
        pthread_exit(thread_return_value_);
    }

private:
    /* The return value that will be used when exiting the thread. */
    void* thread_return_value_;
};

/* Dummy function to simulate a condition for thread exit. */
bool should_exit_thread_immediately()
{
    static int counter = 0;
    counter++;
    return counter > 5; // exits after a few iterations
}

/* Function that does some work and may throw ThreadExitException to exit. */
void do_some_work()
{
    while (1) {
        /* Do some useful things here... */
        std::cout << "Thread working..." << std::endl;

        if (should_exit_thread_immediately())
            throw ThreadExitException(/* thread's return value = */ nullptr);
    }
}

/* Thread entry function. */
void* thread_function(void*)
{
    try {
        do_some_work();
    }
    catch (ThreadExitException& ex) {
        /* Some function indicated that we should exit the thread. */
        ex.DoThreadExit();
    }

    return nullptr;
}

/* Main program. */
int main()
{
    pthread_t thread;

    /* Create the thread. */
    pthread_create(&thread, nullptr, &thread_function, nullptr);

    /* Wait for the thread to finish. */
    pthread_join(thread, nullptr);

    std::cout << "Thread has exited safely." << std::endl;

    return 0;
}

