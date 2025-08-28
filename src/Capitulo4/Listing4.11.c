#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct job {
    struct job* next;
    int id;  /* identificador del trabajo */
};

/* Cola de trabajos pendientes */
struct job* job_queue = NULL;

/* Mutex que protege job_queue */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Función que simula procesar un trabajo */
void process_job(struct job* j) {
    printf("Procesando trabajo %d\n", j->id);
    sleep(1); // simula trabajo
}

/* Hilo que procesa trabajos hasta vaciar la cola */
void* thread_function(void* arg) {
    while (1) {
        struct job* next_job;
        pthread_mutex_lock(&job_queue_mutex);
        if (job_queue == NULL)
            next_job = NULL;
        else {
            next_job = job_queue;
            job_queue = job_queue->next;
        }
        pthread_mutex_unlock(&job_queue_mutex);

        if (next_job == NULL)
            break;

        process_job(next_job);
        free(next_job);
    }
    return NULL;
}

/* ==== MAIN ==== */
int main() {
    pthread_t thread;

    // Crear 3 trabajos y ponerlos en la cola
    for (int i = 1; i <= 3; i++) {
        struct job* new_job = (struct job*)malloc(sizeof(struct job));
        new_job->id = i;
        pthread_mutex_lock(&job_queue_mutex);
        new_job->next = job_queue;
        job_queue = new_job;
        pthread_mutex_unlock(&job_queue_mutex);
    }

    // Crear el hilo procesador
    pthread_create(&thread, NULL, thread_function, NULL);

    // Esperar a que termine
    pthread_join(thread, NULL);

    printf("Todos los trabajos fueron procesados.\n");
    return 0;
}
