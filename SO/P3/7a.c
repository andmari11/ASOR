// system_version.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <comando> [argumentos...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Construir el comando a ejecutar concatenando los argumentos
    // Esto es necesario porque system() toma una cadena completa
    // Por ejemplo, si el usuario ejecuta:
    // ./system_version ls -l /home
    // El comando a ejecutar será "ls -l /home"
    size_t comando_length = 0;
    for (int i = 1; i < argc; i++) {
        comando_length += strlen(argv[i]) + 1; // +1 para el espacio o null terminator
    }

    char *comando = malloc(comando_length);
    if (comando == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    comando[0] = '\0';
    for (int i = 1; i < argc; i++) {
        strcat(comando, argv[i]);
        if (i < argc - 1) {
            strcat(comando, " ");
        }
    }

    // Ejecutar el comando usando system()
    int ret = system(comando);
    free(comando);

    if (ret == -1) {
        perror("system");
        return EXIT_FAILURE;
    }

    // Imprimir la cadena después de la ejecución del comando
    printf("El comando terminó de ejecutarse\n");

    return EXIT_SUCCESS;
}
