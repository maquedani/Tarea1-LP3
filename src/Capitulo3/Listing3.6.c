#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Crear un proceso hijo (ejemplo: Notepad)
    if (!CreateProcess(
            NULL,             // Nombre del programa
            "notepad.exe",    // Comando a ejecutar
            NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) 
    {
        printf("Fallo al crear el proceso hijo.\n");
        return 1;
    }

    printf("Proceso hijo creado con PID %lu\n", pi.dwProcessId);

    // Dormir 60 segundos para simular que el padre está “vivo”
    printf("Padre durmiendo 60 segundos. Observa el proceso hijo...\n");
    Sleep(60000);

    // Cerrar handles del proceso hijo (aquí es cuando Windows limpia)
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Handles cerrados, proceso hijo ya no se mantiene.\n");
    return 0;
}