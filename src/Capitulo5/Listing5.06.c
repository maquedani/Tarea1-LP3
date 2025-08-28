#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int main(int argc, char* const argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    int fd;
    void* file_memory;
    int integer;

    /* Abrir el archivo */
    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    /* Crear el mapeo de memoria */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    /* Leer el entero desde la memoria mapeada */
    sscanf((char*)file_memory, "%d", &integer);
    printf("Valor actual: %d\n", integer);

    /* Escribir el doble en la memoria mapeada */
    sprintf((char*)file_memory, "%d\n", 2 * integer);

    /* Liberar la memoria */
    if (munmap(file_memory, FILE_LENGTH) == -1) {
        perror("munmap");
        return 1;
    }

    return 0;
}
