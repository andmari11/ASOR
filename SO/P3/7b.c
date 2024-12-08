// execvp_version.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <comando> [argumentos...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        // Error al crear el proceso hijo
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // Proceso hijo: Ejecutar el comando usando execvp()
        // Preparar los argumentos para execvp()
        // argv[1] es el comando, argv[2..] son los argumentos
        char **comando = &argv[1];

        // Ejecutar el comando
        if (execvp(comando[0], comando) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE); // Salir si execvp falla
        }
    } else {
        // Proceso padre: Esperar a que el hijo termine
        int estado;
        if (waitpid(pid, &estado, 0) == -1) {
            perror("waitpid");
            return EXIT_FAILURE;
        }

        // Imprimir la cadena después de la ejecución del comando
        printf("El comando terminó de ejecutarse\n");
    }

    return EXIT_SUCCESS;
}
