#include <windows.h>
#include <iostream>

// Function executed by the thread
DWORD WINAPI thread_function(LPVOID lpParam)
{
    std::cerr << "Child thread process ID: " << GetCurrentProcessId() 
              << ", thread ID: " << GetCurrentThreadId() << std::endl;

    // Spin forever
    while (true) { Sleep(1000); } // Sleep to avoid max CPU usage
    return 0;
}

int main()
{
    std::cerr << "Main thread process ID: " << GetCurrentProcessId() 
              << ", thread ID: " << GetCurrentThreadId() << std::endl;

    HANDLE thread = CreateThread(
        nullptr,        // default security attributes
        0,              // default stack size
        thread_function,// thread function
        nullptr,        // parameter to thread function
        0,              // default creation flags
        nullptr         // ignore thread ID
    );

    if (thread == nullptr) {
        std::cerr << "Failed to create thread." << std::endl;
        return 1;
    }

    // Spin forever
    while (true) { Sleep(1000); }

    // Close thread handle (not reached in this example)
    CloseHandle(thread);
    return 0;
}
