#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct job {
    /* Link field for linked list. */
    struct job* next;
    /* Other fields describing work to be done... */
    int job_id; // Campo de ejemplo
};

/* A linked list of pending jobs. */
struct job* job_queue;

/* Función para procesar un trabajo */
void process_job(struct job* j) {
    printf("Procesando trabajo %d\n", j->job_id);
}

/* Process queued jobs until the queue is empty. */

void* thread_function (void* arg)
{
    while (job_queue != NULL) {
        /* Get the next available job. */
        struct job* next_job = job_queue;
        /* Remove this job from the list. */
        job_queue = job_queue->next;
        /* Carry out the work. */
        process_job (next_job);
        /* Clean up. */
        free (next_job);
    }
    return NULL;
}

/* Función para añadir trabajos a la cola */
void add_job(int id) {
    struct job* new_job = (struct job*)malloc(sizeof(struct job));
    new_job->job_id = id;
    new_job->next = job_queue;
    job_queue = new_job;
}

int main() {
    pthread_t tid;

    /* Agregamos algunos trabajos */
    for (int i = 1; i <= 5; i++) {
        add_job(i);
    }

    /* Creamos un hilo para procesar los trabajos */
    if (pthread_create(&tid, NULL, thread_function, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    /* Esperamos a que termine */
    pthread_join(tid, NULL);

    printf("Todos los trabajos procesados.\n");

    return 0;
}