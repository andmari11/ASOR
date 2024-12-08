#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <limits.h>
#include <fcntl.h>

void mostrar_atributos_proceso(const char *proceso) {
    pid_t pid = getpid();   // Identificador del proceso
    pid_t ppid = getppid(); // Identificador del proceso padre
    pid_t pgid = getpgid(pid); // Identificador del grupo de procesos
    pid_t sid = getsid(pid);   // Identificador de la sesión

    // Obtener el directorio de trabajo actual
    char cwd[255];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error al obtener el directorio de trabajo");
        return;
    }

    printf("[%s] PID: %d, PPID: %d, PGID: %d, SID: %d, Directorio: %s\n", proceso, pid, ppid, pgid, sid, cwd);
}

int main() {
    pid_t pid;

    // Crear un nuevo proceso
    pid = fork();

    if (pid < 0) {
        perror("Error al hacer fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Proceso padre: esperar unos segundos antes de finalizar
        printf("Proceso padre\n");
        sleep(5);  // Pausa para observar los estados con `ps`
        mostrar_atributos_proceso("Padre");
        exit(EXIT_SUCCESS);  // El proceso padre termina
    }

    // Proceso hijo (demonio)
    printf("Proceso hijo (demonio)\n");

    // Crear una nueva sesión para el proceso hijo
    if (setsid() < 0) {
        perror("Error al crear una nueva sesión");
        exit(EXIT_FAILURE);
    }

    // Cambiar el directorio de trabajo a /tmp
    if (chdir("/tmp") < 0) {
        perror("Error al cambiar el directorio de trabajo a /tmp");
        exit(EXIT_FAILURE);
    }

    // Esperar antes de continuar para observar los atributos
    sleep(10);  // Pausa para observar los estados con `ps`

    // Mostrar atributos del proceso hijo (demonio)
    mostrar_atributos_proceso("Hijo (Demonio)");

    // El proceso demonio continúa ejecutándose
    while (1) {
        sleep(10);  // El demonio sigue funcionando indefinidamente
    }

    return 0;
}
