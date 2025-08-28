#include <pthread.h>
#include <iostream>
#include <unistd.h>

// ==== tu clase del libro ====
class ThreadExitException {
public:
    ThreadExitException(void* return_value)
        : thread_return_value_(return_value) {}

    void* DoThreadExit() {
        pthread_exit(thread_return_value_);
        return nullptr; // nunca llega
    }

private:
    void* thread_return_value_;
};

bool should_exit_thread_immediately() {
    static int counter = 0;
    return ++counter > 3;  // salir después de 3 iteraciones
}

void do_some_work() {
    while (1) {
        std::cout << "Trabajando...\n";
        sleep(1);
        if (should_exit_thread_immediately())
            throw ThreadExitException(NULL);
    }
}

void* thread_function(void*) {
    try {
        do_some_work();
    }
    catch (ThreadExitException& ex) {
        ex.DoThreadExit();
    }
    return NULL;
}

// ==== AQUI VIENE EL MAIN ====
int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);
    std::cout << "Hilo finalizado.\n";
    return 0;
}
