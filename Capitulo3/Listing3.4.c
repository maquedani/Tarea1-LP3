#include <stdio.h>
#include <stdlib.h>
#include <process.h>  /* For _spawnvp */

/* Spawn a child process running a new program. PROGRAM is the name
 of the program to run; the path will be searched for this program.
 ARG_LIST is a NULL-terminated list of character strings to be
 passed as the program's argument list. Returns the process ID of
 the spawned process. */
int spawn (char* program, char** arg_list) 
{
    /* In Windows, use _spawnvp instead of fork + execvp. */
    int child_pid = _spawnvp(_P_NOWAIT, program, (const char* const*)arg_list);
    if (child_pid == -1) {
        fprintf(stderr, "an error occurred in _spawnvp\n");
        abort();
    }
    return child_pid;
}

int main ()
{
    /* The argument list to pass to the "ls" command. */
    char* arg_list[] = {
        "cmd.exe",   /* argv[0], the name of the program. */
        "/c",
        "dir",
        NULL /* The argument list must end with a NULL. */
    };
    /* Spawn a child process running the "ls" command. Ignore the
    returned child process ID. */
    spawn ("cmd.exe", arg_list);
    printf ("done with main program\n");
    return 0;
}
