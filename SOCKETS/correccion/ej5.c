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

    
    return 0;
}