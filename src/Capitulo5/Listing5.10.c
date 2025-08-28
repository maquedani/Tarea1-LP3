#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Read text from the socket and print it out. Continue until the
   socket closes. Return nonzero if the client sent a "quit"
   message, zero otherwise. */
int server(int client_socket)
{
    while (1) {
        int length;
        char* text;

        /* First, read the length of the text message from the socket. */
        if (read(client_socket, &length, sizeof(length)) == 0)
            return 0;

        /* Allocate a buffer to hold the text, +1 for null terminator */
        text = (char*) malloc(length + 1);
        if (!text) {
            perror("malloc");
            return 0;
        }

        /* Read the text itself */
        if (read(client_socket, text, length) != length) {
            free(text);
            return 0;
        }

        /* Null-terminate to safely print */
        text[length] = '\0';
        printf("%s\n", text);

        /* Check if client sent "quit" */
        if (strcmp(text, "quit") == 0) {
            free(text);
            return 1;
        }

        free(text);
    }
}

int main(int argc, char* const argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <socket>\n", argv[0]);
        return 1;
    }

    const char* const socket_name = argv[1];
    int socket_fd;
    struct sockaddr_un name;
    int client_sent_quit_message;

    /* Create the socket */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        return 1;
    }

    /* Indicate that this is a server */
    memset(&name, 0, sizeof(struct sockaddr_un));
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, socket_name, sizeof(name.sun_path) - 1);

    /* Bind */
    unlink(socket_name); // Remove old socket if exists
    if (bind(socket_fd, (struct sockaddr*)&name, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        close(socket_fd);
        return 1;
    }

    /* Listen for connections */
    if (listen(socket_fd, 5) == -1) {
        perror("listen");
        close(socket_fd);
        return 1;
    }

    /* Accept connections in a loop */
    do {
        struct sockaddr_un client_name;
        socklen_t client_name_len = sizeof(struct sockaddr_un);
        int client_socket_fd;

        client_socket_fd = accept(socket_fd, (struct sockaddr*)&client_name, &client_name_len);
        if (client_socket_fd == -1) {
            perror("accept");
            continue;
        }

        client_sent_quit_message = server(client_socket_fd);

        close(client_socket_fd);

    } while (!client_sent_quit_message);

    close(socket_fd);
    unlink(socket_name);
    return 0;
}
