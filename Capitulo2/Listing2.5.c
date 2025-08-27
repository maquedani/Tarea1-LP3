#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* A handle for a temporary file created with write_temp_file. */
typedef FILE* temp_file_handle;

temp_file_handle write_temp_file(char* buffer, size_t length) {
    char temp_path[MAX_PATH];
    char temp_filename[MAX_PATH];

    // Get temp directory
    GetTempPathA(MAX_PATH, temp_path);

    // Create unique filename
    GetTempFileNameA(temp_path, "tmp", 0, temp_filename);

    // Open file
    FILE* f = fopen(temp_filename, "w+b");
    if (!f) {
        perror("fopen");
        exit(1);
    }

    // Write length
    fwrite(&length, sizeof(length), 1, f);

    // Write data
    fwrite(buffer, 1, length, f);

    // Rewind so it's ready for reading
    rewind(f);

    // Delete file from filesystem, but keep handle open
    DeleteFileA(temp_filename);

    return f;
}

char* read_temp_file(temp_file_handle temp_file, size_t* length) {
    char* buffer;

    // Read length
    fread(length, sizeof(*length), 1, temp_file);

    // Allocate memory
    buffer = (char*)malloc(*length);

    // Read data
    fread(buffer, 1, *length, temp_file);

    // Close file
    fclose(temp_file);

    return buffer;
}

int main() {
    char* data = "Hola desde Windows!";
    size_t length = strlen(data) + 1;

    temp_file_handle tmp = write_temp_file(data, length);

    size_t read_length;
    char* buffer = read_temp_file(tmp, &read_length);

    printf("Leído del archivo temporal (%zu bytes): %s\n", read_length, buffer);

    free(buffer);
    return 0;
}

