#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main ()
{
    int fds[2];
    pid_t pid;

    /* Crear el pipe */
    pipe(fds);

    /* Crear el proceso hijo */
    pid = fork();

    if (pid == (pid_t) 0) {
        /* Proceso hijo */
        close(fds[1]);              // cerrar extremo de escritura
        dup2(fds[0], STDIN_FILENO); // conectar lectura del pipe a stdin
        close(fds[0]);              // cerrar descriptor original

        /* Ejecutar sort */
        execlp("sort", "sort", (char *)NULL);
        perror("execlp falló");     // solo si execlp falla
        return 1;
    } else {
        /* Proceso padre */
        FILE* stream;

        close(fds[0]);              // cerrar extremo de lectura

        /* Convertir descriptor a FILE* y escribir líneas */
        stream = fdopen(fds[1], "w");
        fprintf(stream, "This is a test.\n");
        fprintf(stream, "Hello, world.\n");
        fprintf(stream, "My dog has fleas.\n");
        fprintf(stream, "This program is great.\n");
        fprintf(stream, "One fish, two fish.\n");
        fflush(stream);
        close(fds[1]);

        /* Esperar a que termine el hijo */
        waitpid(pid, NULL, 0);
    }

    return 0;
}
