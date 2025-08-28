#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/* ==== Código del libro ==== */
int thread_flag;
pthread_mutex_t thread_flag_mutex;

void initialize_flag() {
    pthread_mutex_init(&thread_flag_mutex, NULL);
    thread_flag = 0;
}

/* Simulación de trabajo */
void do_work() {
    printf("Hilo trabajando...\n");
    sleep(1);
}

/* Hilo que solo trabaja cuando thread_flag = 1 */
void* thread_function(void* thread_arg) {
    while (1) {
        int flag_is_set;
        pthread_mutex_lock(&thread_flag_mutex);
        flag_is_set = thread_flag;
        pthread_mutex_unlock(&thread_flag_mutex);

        if (flag_is_set)
            do_work();
        else
            usleep(100000); // 0.1s -> evita consumir CPU girando en vacío
    }
    return NULL;
}

/* Cambia el valor del flag */
void set_thread_flag(int flag_value) {
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* ==== MAIN ==== */
int main() {
    pthread_t thread;
    initialize_flag();

    // Crear el hilo
    pthread_create(&thread, NULL, thread_function, NULL);

    // Probar el flag
    printf("Activando el flag...\n");
    set_thread_flag(1);
    sleep(3);

    printf("Desactivando el flag...\n");
    set_thread_flag(0);
    sleep(2);

    printf("Activando el flag otra vez...\n");
    set_thread_flag(1);
    sleep(2);

    // Terminar el programa (forzamos, porque el hilo es infinito)
    printf("Fin del main.\n");
    return 0;
}
