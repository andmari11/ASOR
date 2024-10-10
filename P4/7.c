#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    // Crear la tubería
    int p_h[2]; //0 lectura 1 escritura
    int h_p[2];
    if (pipe(p_h) == -1 || pipe(h_p) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Proceso hijo
        close(p_h[1]);
        close(h_p[0]);
        char buffer[255];
        int message_count = 0;
        while (1) {
            read(p_h[0], buffer, 255);
            printf("Hijo recibió: %s\n", buffer);

            message_count++;

            if (message_count >= 10) {
                // Enviar señal de finalización ('q') después de 10 mensajes
                write(h_p[1], "q", 1);
                break;
            } else {
                // Esperar 1 segundo
                sleep(1);
                // Enviar confirmación ('l') al padre
                write(h_p[1], "l", 1);
            }
        }
        

    } 
    else {  // Proceso padre
        close(p_h[0]);
        close(h_p[1]);

        while(1){
            char cin[255];
            fgets(cin, 255, stdin);
            write(p_h[1], cin, strlen(cin) + 1);
            char response;
            read(h_p[0], &response, 1);

            if (response == 'q') {
                printf("Padre: El hijo solicitó finalizar.\n");
                break;
            } else if (response == 'l') {
                printf("Padre: Hijo listo para recibir el siguiente mensaje.\n");
            }
        }


    }

    // Esperar a que ambos procesos terminen
    wait(NULL);

    return 0;
}
