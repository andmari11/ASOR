// Ejercicio 5. Convertir el servidor UDP en multi-proceso siguiendo el patrón pre-fork. Una vez
// asociado el socket a la dirección local con bind(2), crear varios procesos que llamen a recvfrom(2)
// de forma que cada uno atenderá un mensaje de forma concurrente. Imprimir el PID del proceso
// servidor para comprobarlo. Para terminar el servidor, enviar la señal SIGTERM al grupo de procesos.




#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


// Manejador de la señal SIGCHLD para evitar procesos zombies
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Formato incorrecto. Introduce una dirección IPv4 o IPv6 y un puerto como comandos\n");
        return -1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // Permite IPv4 o IPv6
    hints.ai_socktype = SOCK_DGRAM; // Tipo de socket UDP
    hints.ai_flags = AI_PASSIVE; // Usar la dirección IP del servidor

    struct addrinfo *result;
    // Obtiene la información de la dirección
    if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0) {
        perror("getaddrinfo()");
        return -1;
    }

    // Crea el socket
    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    // Asocia el socket a la dirección y puerto especificados
    if (bind(sd, result->ai_addr, result->ai_addrlen) != 0) {
        perror("bind()");
        return -1;
    }
    freeaddrinfo(result);

    // Configura el manejador para la señal SIGCHLD
    signal(SIGCHLD, handle_sigchld);

    char comando[2]; // Buffer para el comando
    char host[NI_MAXHOST]; // Buffer para el nombre del host del cliente
    char serv[NI_MAXSERV]; // Buffer para el nombre del servicio del cliente

    struct sockaddr_storage client_addr; // Dirección del cliente
    socklen_t client_len = sizeof(client_addr); // Longitud de la dirección del cliente

    int status;
    for(int i = 0; i < 2; i++) {
        pid_t pid=fork();
        if(pid==0) {
            while (1) {
                // Recibe el comando del cliente
                ssize_t bytes = recvfrom(sd, comando, 2, 0, (struct sockaddr *) &client_addr, &client_len);
                if (bytes == -1) {
                    perror("recvfrom()");
                    return -1;
                }
                comando[1] = '\0';
                // Obtiene el nombre del host y del servicio del cliente
                getname(info((struct sockaddr *) &client_addr, client_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV));

                printf("%i byte(s) de %s:%s cuyo PID: %d\n", bytes, host, serv, getpid());
                time_t tiempo = time(NULL);
                struct tm *tm = localtime(&tiempo);
                
                // Procesa el comando y envía la respuesta al cliente
                char *respuesta = process_command(comando[0], tm);
                sendto(sd, respuesta, strlen(respuesta), 0, (struct sockaddr *) &client_addr, client_len);
            }
        }
        else{
            pid=wait(&status);
        }
    }
    return 0;
}

char* process_command(char comando, struct tm *tm) {
    static char buf[50];
    size_t bytes;

    switch(comando) {
        case 't':
            bytes = strftime(buf, 49, "%I:%M:%S %p", tm);
            buf[bytes] = '\0';
            break;
        case 'd':
            bytes = strftime(buf, 49, "%Y-%m-%d", tm);
            buf[bytes] = '\0';
            break;
        default:
            strcpy(buf, "Comando no soportado.");
            break;
    }

    return buf;
}