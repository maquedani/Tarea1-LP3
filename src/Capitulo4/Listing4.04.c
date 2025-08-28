#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Compute successive prime numbers (inefficiently). Return the
   Nth prime number, where N is the value pointed to by *ARG. */
void* compute_prime(void* arg)
{
    int candidate = 2;
    int n = *((int*) arg);
    int* result = malloc(sizeof(int)); // Guardar el resultado en memoria dinámica

    if (!result) {
        perror("malloc");
        pthread_exit(NULL);
    }

    while (1) {
        int factor;
        int is_prime = 1;
        /* Test primality by successive division. */
        for (factor = 2; factor < candidate; ++factor) {
            if (candidate % factor == 0) {
                is_prime = 0;
                break;
            }
        }
        /* Is this the prime number we're looking for? */
        if (is_prime) {
            if (--n == 0) {
                *result = candidate; // Guardar el prime en la memoria dinámica
                pthread_exit(result); // Retornar puntero al resultado
            }
        }
        ++candidate;
    }
}

int main()
{
    pthread_t thread;
    int which_prime = 5000;
    int* prime;

    /* Start the computing thread, up to the 5,000th prime number. */
    pthread_create(&thread, NULL, compute_prime, &which_prime);

    /* Do some other work here... */
    printf("Computing the %dth prime number...\n", which_prime);

    /* Wait for the prime number thread to complete, and get the result. */
    pthread_join(thread, (void**) &prime);

    /* Print the largest prime it computed. */
    printf("The %dth prime number is %d.\n", which_prime, *prime);

    free(prime); // Liberar la memoria asignada por el hilo
    return 0;
}
