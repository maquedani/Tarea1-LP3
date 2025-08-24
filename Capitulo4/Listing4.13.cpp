#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>

// Thread flag variable
int thread_flag;

// Mutex protecting the thread flag
std::mutex thread_flag_mutex;

// Function to initialize the flag
void initialize_flag()
{
    // Initialize the mutex (in C++ we don't need explicit initialization, comment left for clarity)
    thread_flag = 0;
}

// Function that simulates work for the thread
void do_work()
{
    // Perform some useful work here
    std::cout << "Thread is doing work..." << std::endl;
}

// Thread logic: repeatedly calls do_work while the thread flag is set
void thread_function()
{
    while (true)
    {
        int flag_is_set;

        // Protect flag access with a lock
        {
            std::lock_guard<std::mutex> lock(thread_flag_mutex);
            flag_is_set = thread_flag;
        }

        if (flag_is_set)
            do_work();
        else
        {
            // Else don't do anything. Just loop again
            std::this_thread::yield(); // yield CPU
        }
    }
}

// Sets the value of the thread flag
void set_thread_flag(int flag_value)
{
    // Protect the flag with a lock
    std::lock_guard<std::mutex> lock(thread_flag_mutex);
    thread_flag = flag_value;
}

int main()
{
    // Initialize the thread flag
    initialize_flag();

    // Create the thread
    std::thread worker(thread_function);

    // Activate the flag so the thread does work
    set_thread_flag(1);

    // Let the thread work a little
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Deactivate the flag
    set_thread_flag(0);

    // Let it loop a little more
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // As the thread is infinite, detach it (just for this example)
    worker.detach(); // allow it to run in the background

    return 0;
}
