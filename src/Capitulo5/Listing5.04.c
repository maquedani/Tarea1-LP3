#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

/* ==== Código del libro ==== */
int binary_semaphore_wait(int semid) {
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = -1;  // decrementa
    operations[0].sem_flg = SEM_UNDO;
    return semop(semid, operations, 1);
}

int binary_semaphore_post(int semid) {
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;   // incrementa
    operations[0].sem_flg = SEM_UNDO;
    return semop(semid, operations, 1);
}

/* ==== MAIN ==== */
int main() {
    key_t key = 5678;
    int sem_flags = IPC_CREAT | 0666;

    // Crear semáforo binario
    int semid = semget(key, 1, sem_flags);
    if (semid == -1) {
        perror("Error creando semáforo");
        exit(1);
    }

    // Inicializar el semáforo a 1 (disponible)
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short int *array;
        struct seminfo *__buf;
    } arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("Error inicializando semáforo");
        exit(1);
    }

    printf("Esperando semáforo...\n");
    binary_semaphore_wait(semid);   // decrementa a 0
    printf("Semáforo adquirido!\n");

    printf("Liberando semáforo...\n");
    binary_semaphore_post(semid);   // incrementa a 1
    printf("Semáforo liberado!\n");

    // Liberar semáforo
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("Error liberando semáforo");
        exit(1);
    }

    return 0;
}
