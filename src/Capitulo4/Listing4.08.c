#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Allocate a temporary buffer. */
void* allocate_buffer (size_t size)
{
    return malloc (size);
}

/* Deallocate a temporary buffer. */
void deallocate_buffer (void* buffer)
{
    free (buffer);
}

void do_some_work ()
{
    /* Allocate a temporary buffer. */
    void* temp_buffer = allocate_buffer (1024);

    /* Register a cleanup handler for this buffer, to deallocate it in
    case the thread exits or is cancelled. */

    pthread_cleanup_push (deallocate_buffer, temp_buffer);

    /* Do some work here that might call pthread_exit or might be
    cancelled... */
    
    /* Simulamos trabajo del hilo */
    printf("Hilo trabajando...\n");
    
    /* Unregister the cleanup handler. Because we pass a nonzero value,
    this actually performs the cleanup by calling
    deallocate_buffer. */

    pthread_cleanup_pop (1);
}

/*Agregado para su prueba de ejecución*/
void* thread_func(void* arg)
{
    do_some_work();
    return NULL;
}

int main()
{
    pthread_t tid;

    /* Creamos un hilo */
    if (pthread_create(&tid, NULL, thread_func, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    /* Esperamos a que termine el hilo */
    pthread_join(tid, NULL);

    printf("Hilo finalizado.\n");

    return 0;
}