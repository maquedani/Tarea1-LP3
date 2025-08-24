#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Para sleep

/* Estructura de un trabajo */
struct job {
    struct job* next;    // Siguiente trabajo en la cola
    int job_id;          // ID del trabajo
};

/* Cola de trabajos global */
struct job* job_queue = NULL;

/* Mutex para proteger la cola */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Función para simular el procesamiento de un trabajo */
void process_job(struct job* j) {
    printf("Processing job %d\n", j->job_id);
    sleep(1); // Simula tiempo de trabajo
}

/* Función del hilo */
void* thread_function(void* arg) {
    while (1) {
        struct job* next_job;

        pthread_mutex_lock(&job_queue_mutex); // Bloquea la cola
        if (job_queue == NULL)
            next_job = NULL;                  // Cola vacía
        else {
            next_job = job_queue;             // Toma el primer trabajo
            job_queue = job_queue->next;      // Lo elimina de la cola
        }
        pthread_mutex_unlock(&job_queue_mutex); // Desbloquea la cola

        if (next_job == NULL)
            break; // No hay más trabajos, termina hilo

        process_job(next_job);
        free(next_job);
    }
    return NULL;
}

/* Función para agregar un trabajo a la cola */
void add_job(int job_id) {
    struct job* new_job = (struct job*)malloc(sizeof(struct job));
    new_job->job_id = job_id;
    new_job->next = NULL;

    pthread_mutex_lock(&job_queue_mutex);
    if (job_queue == NULL)
        job_queue = new_job;
    else {
        struct job* tmp = job_queue;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_job;
    }
    pthread_mutex_unlock(&job_queue_mutex);
}

int main() {
    const int NUM_THREADS = 3;
    const int NUM_JOBS = 10;
    pthread_t threads[NUM_THREADS];

    // Agregar trabajos a la cola
    for (int i = 1; i <= NUM_JOBS; i++)
        add_job(i);

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, thread_function, NULL);

    // Esperar que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("All jobs processed.\n");
    return 0;
}
