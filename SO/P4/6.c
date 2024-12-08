#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s comando1 arg1 | comando2 arg2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Crear la tubería
    int pipefd[2]; // pipefd[0] es para lectura, pipefd[1] para escritura
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Crear el proceso hijo
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Proceso hijo
        // Redirigir la entrada estándar (stdin) al extremo de lectura de la tubería
        close(pipefd[1]); // Cerrar el extremo de escritura de la tubería, ya que no se usa
        dup2(pipefd[0], STDIN_FILENO); // Reemplazar stdin por el extremo de lectura
        close(pipefd[0]); // Cerrar el descriptor original del extremo de lectura

        // Ejecutar comando2 argumento2
        char *comando2[] = {argv[3], argv[4], NULL};
        execvp(comando2[0], comando2);
        perror("execvp comando2");
        exit(EXIT_FAILURE);
    } else {  // Proceso padre
        // Redirigir la salida estándar (stdout) al extremo de escritura de la tubería
        close(pipefd[0]); // Cerrar el extremo de lectura de la tubería, ya que no se usa
        dup2(pipefd[1], STDOUT_FILENO); // Reemplazar stdout por el extremo de escritura
        close(pipefd[1]); // Cerrar el descriptor original del extremo de escritura

        // Ejecutar comando1 argumento1
        char *comando1[] = {argv[1], argv[2], NULL};
        execvp(comando1[0], comando1);
        perror("execvp comando1");
        exit(EXIT_FAILURE);
    }

    // Esperar a que ambos procesos terminen
    wait(NULL);

    return 0;
}
