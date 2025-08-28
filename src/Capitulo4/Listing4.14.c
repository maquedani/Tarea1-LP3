#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/* ==== Código del libro ==== */
int thread_flag;
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

void initialize_flag() {
    pthread_mutex_init(&thread_flag_mutex, NULL);
    pthread_cond_init(&thread_flag_cv, NULL);
    thread_flag = 0;
}

/* Simula trabajo */
void do_work() {
    printf("Hilo trabajando...\n");
    sleep(1);
}

/* Hilo que hace trabajo solo cuando flag = 1, bloquea si flag = 0 */
void* thread_function(void* thread_arg) {
    while (1) {
        pthread_mutex_lock(&thread_flag_mutex);
        while (!thread_flag)
            pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);
        pthread_mutex_unlock(&thread_flag_mutex);

        do_work();
    }
    return NULL;
}

/* Cambia el valor del flag y despierta al hilo si está esperando */
void set_thread_flag(int flag_value) {
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_cond_signal(&thread_flag_cv);
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* ==== MAIN ==== */
int main() {
    pthread_t thread;
    initialize_flag();

    // Crear el hilo
    pthread_create(&thread, NULL, thread_function, NULL);

    // Activar el flag y dejar que trabaje 3 segundos
    printf("Activando flag...\n");
    set_thread_flag(1);
    sleep(3);

    // Desactivar flag (aunque el hilo seguirá esperando)
    printf("Desactivando flag...\n");
    set_thread_flag(0);
    sleep(2);

    // Activar flag otra vez
    printf("Activando flag otra vez...\n");
    set_thread_flag(1);
    sleep(2);

    printf("Fin del main.\n");

    // Nota: el hilo es infinito, aquí el programa termina forzadamente
    return 0;
}
