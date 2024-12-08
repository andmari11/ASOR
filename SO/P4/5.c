#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

volatile sig_atomic_t delete_file = 1;  // Bandera para controlar si se borra el archivo

// Manejador de la señal SIGUSR1: evita que se borre el archivo
void sigusr1_handler(int sig) {
    delete_file = 0;  // Si se recibe SIGUSR1, no se borra el archivo
}

// Manejador de la señal SIGALRM: indica que ha expirado el tiempo
void sigalrm_handler(int sig) {
    // No hace falta hacer nada, solo salir de sigsuspend
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <segundos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        fprintf(stderr, "El número de segundos debe ser mayor que 0.\n");
        return EXIT_FAILURE;
    }

    // Nombre del archivo ejecutable
    char *exec_name = argv[0];

    // Registrar el manejador para SIGUSR1
    struct sigaction sa_usr1;
    sa_usr1.sa_handler = sigusr1_handler;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction SIGUSR1");
        exit(EXIT_FAILURE);
    }

    // Registrar el manejador para SIGALRM
    struct sigaction sa_alrm;
    sa_alrm.sa_handler = sigalrm_handler;
    sigemptyset(&sa_alrm.sa_mask);
    sa_alrm.sa_flags = 0;
    if (sigaction(SIGALRM, &sa_alrm, NULL) == -1) {
        perror("sigaction SIGALRM");
        exit(EXIT_FAILURE);
    }

    // Bloquear señales para usar sigsuspend
    sigset_t blk_set, old_set;
    sigemptyset(&blk_set);
    sigaddset(&blk_set, SIGALRM);  // Bloquear SIGALRM temporalmente
    sigaddset(&blk_set, SIGUSR1);  // Bloquear SIGUSR1 temporalmente
    sigprocmask(SIG_BLOCK, &blk_set, &old_set);  // Bloquear señales

    // Configurar la alarma
    alarm(seconds);

    // Esperar la señal
    sigset_t suspend_set;
    sigemptyset(&suspend_set);  // Conjunto vacío, espera cualquier señal desbloqueada
    sigsuspend(&suspend_set);   // Suspender hasta que se reciba una señal

    // Restaurar la máscara de señales
    sigprocmask(SIG_SETMASK, &old_set, NULL);

    // Si la bandera delete_file es 1, procedemos a borrar el archivo
    if (delete_file) {
        printf("Tiempo expirado. Borrando el archivo %s...\n", exec_name);
        if (unlink(exec_name) == -1) {
            perror("Error al borrar el archivo");
        }
    } else {
        printf("Se recibió SIGUSR1. Cancelando el borrado.\n");
    }

    return EXIT_SUCCESS;
}
