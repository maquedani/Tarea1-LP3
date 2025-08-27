#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* We must define union semun ourselves. */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Initialize a binary semaphore with a value of 1. */
int binary_semaphore_initialize (int semid)
{
    union semun argument;
    unsigned short values[1];
    values[0] = 1;
    argument.array = values;
    return semctl (semid, 0, SETALL, argument);
} 

/* Main agregado */
int main()
{
    key_t key = ftok("semfile", 65); // archivo y id para generar key
    int semid;

    /* Crear un semáforo con 1 semáforo en el conjunto */
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    /* Inicializar semáforo binario */
    if (binary_semaphore_initialize(semid) == -1) {
        perror("semctl");
        exit(1);
    }

    printf("Semáforo binario inicializado.\n");

    /* semop() para bloquear/desbloquear */

    /* Eliminar el semáforo */
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl remove");
        exit(1);
    }

    printf("Semáforo eliminado.\n");
    return 0;
}