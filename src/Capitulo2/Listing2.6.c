#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 

char* read_from_file (const char* filename, size_t length)
{
    char* buffer;
    int fd;
    ssize_t bytes_read;
    /* Allocate the buffer. */
    buffer = (char*) malloc (length);
    if (buffer == NULL)
        return NULL;
    /* Open the file. */
    fd = open (filename, O_RDONLY);
    if (fd == -1) {
    /* open failed. Deallocate buffer before returning. */
        free (buffer);
        return NULL;
    }
    /* Read the data. */
    bytes_read = read (fd, buffer, length);
    if (bytes_read != length) {
        /* read failed. Deallocate buffer and close fd before returning. */
        free (buffer);
        close (fd);
        return NULL;
    }
    /* Everything's fine. Close the file and return the buffer. */
    close (fd);
    return buffer;
}

/*Main agregado para la ejecución del listing*/
int main() {
    char* data = read_from_file("Listing2.6_Prueba.txt", 20);  // lee 20 bytes del archivo
    if (data == NULL) {
        printf("Error leyendo archivo\n");
        return 1;
    }

    data[19] = '\0';  // fin de cadena
    printf("Contenido leido: %s\n", data);

    free(data);
    return 0;
}