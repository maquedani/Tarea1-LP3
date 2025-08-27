#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

sig_atomic_t child_exit_status;

void clean_up_child_process (int signal_number)
{
    /* Clean up the child process. */
    int status;
    wait (&status);
    /* Store its exit status in a global variable. */
    child_exit_status = status;
}

int main ()
{
    /* Handle SIGCHLD by calling clean_up_child_process. */
    struct sigaction sigchld_action;
    memset (&sigchld_action, 0, sizeof (sigchld_action));
    sigchld_action.sa_handler = &clean_up_child_process;
    sigaction (SIGCHLD, &sigchld_action, NULL);
    
    /* Now do things, including forking a child process. */
    /* Agregado */
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo
        printf("Soy el hijo, voy a terminar con código 42\n");
        exit(42);
    }

    // Proceso padre: esperar un poco para que el hijo termine
    sleep(2);
    printf("Estado guardado del hijo: %d\n", WEXITSTATUS(child_exit_status));
    /* ... */

    return 0;
} 