#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAXBUF 1024

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <dirección servidor> <puerto servidor> <comando>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Variables para el socket
    int sockfd;
    struct sockaddr_storage server_addr;
    socklen_t server_len;
    struct addrinfo hints, *res;

    // El comando que el cliente enviará al servidor
    char *command = argv[3];

    // Preparar la estructura hints para getaddrinfo
    hints.ai_family = AF_UNSPEC;       // Acepta tanto IPv4 como IPv6
    hints.ai_socktype = SOCK_DGRAM;    // UDP
    hints.ai_protocol = IPPROTO_UDP;   // UDP

    // Obtener la información de la dirección del servidor
    int status = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (status != 0) {
        fprintf(stderr, "Error getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Crear el socket UDP
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        handle_error("socket");
    }

    // Preparar la dirección del servidor
    memcpy(&server_addr, res->ai_addr, res->ai_addrlen);
    server_len = res->ai_addrlen;
    freeaddrinfo(res);  // Ya no necesitamos la estructura de direcciones

    // Enviar el comando al servidor
    if (sendto(sockfd, command, strlen(command), 0, (struct sockaddr *)&server_addr, server_len) == -1) {
        handle_error("sendto");
    }

    // Recibir la respuesta del servidor
    char buffer[MAXBUF];
    int n = recvfrom(sockfd, buffer, MAXBUF, 0, (struct sockaddr *)&server_addr, &server_len);
    if (n == -1) {
        handle_error("recvfrom");
    }
    buffer[n] = '\0';  // Asegurarse de que la respuesta está correctamente terminada

    // Imprimir la respuesta del servidor
    printf("Respuesta del servidor: %s\n", buffer);

    // Cerrar el socket
    close(sockfd);

    return 0;
}
