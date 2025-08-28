#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

/* Definición de union semun */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Obtener un semáforo binario, asignando si es necesario */
int binary_semaphore_allocation(key_t key, int sem_flags) {
    return semget(key, 1, sem_flags);
}

/* Liberar un semáforo binario */
int binary_semaphore_deallocate(int semid) {
    union semun ignored_argument;
    return semctl(semid, 0, IPC_RMID, ignored_argument);
}

/* ==== MAIN ==== */
int main() {
    key_t key = 1234; // clave arbitraria para el semáforo
    int sem_flags = IPC_CREAT | 0666;

    // Crear o abrir el semáforo
    int semid = binary_semaphore_allocation(key, sem_flags);
    if (semid == -1) {
        perror("Error creando semáforo");
        exit(1);
    }
    printf("Semáforo creado con ID: %d\n", semid);

    // Inicializar el valor del semáforo a 1 (disponible)
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("Error inicializando semáforo");
        exit(1);
    }
    printf("Semáforo inicializado a 1.\n");

    // Aquí podrías hacer operaciones de P/V (semop) si quisieras

    // Liberar el semáforo
    if (binary_semaphore_deallocate(semid) == -1) {
        perror("Error liberando semáforo");
        exit(1);
    }
    printf("Semáforo liberado correctamente.\n");

    return 0;
}
