#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* thread_arg)
{
    for (int i = 1; i <= 5; i++) {
        printf("Detached thread working: step %d\n", i);
        sleep(1);  // Simula trabajo
    }
    printf("Detached thread finished.\n");
    return NULL;
}

int main()
{
    pthread_attr_t attr;
    pthread_t thread;

    /* Inicializar atributos del hilo y ponerlo en detached */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    /* Crear hilo detached */
    pthread_create(&thread, &attr, &thread_function, NULL);

    /* Destruir los atributos */
    pthread_attr_destroy(&attr);

    /* Trabajo del hilo principal */
    for (int i = 1; i <= 5; i++) {
        printf("Main thread working: step %d\n", i);
        sleep(1);  // Simula trabajo
    }
    printf("Main thread finished.\n");

    /* No hace falta join porque el hilo es detached */
    return 0;
}
