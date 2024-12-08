#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/socket.h>

#define MAXBUF 1024

void print_client_info(struct sockaddr_storage *client_addr) {
    char host[NI_MAXHOST], service[NI_MAXSERV];
    int ret = getnameinfo((struct sockaddr *)client_addr, sizeof(struct sockaddr_storage), host, sizeof(host),
                          service, sizeof(service), NI_NUMERICHOST | NI_NUMERICSERV);
    if (ret == 0) {
        printf("Cliente: %s:%s\n", host, service);
    } else {
        fprintf(stderr, "Error en getnameinfo: %s\n", gai_strerror(ret));
    }
}

void handle_client(int sockfd, struct sockaddr_storage *client_addr, socklen_t client_len) {
    char buffer[MAXBUF];
    int n;
    
    // Recibir mensaje del cliente
    n = recvfrom(sockfd, buffer, MAXBUF, 0, (struct sockaddr *)client_addr, &client_len);
    if (n == -1) {
        perror("recvfrom");
        return;
    }

    buffer[n] = '\0';  // Asegurarse de que el mensaje está terminado en '\0'
    
    // Mostrar la dirección y puerto del cliente
    print_client_info(client_addr);

    // Procesar el comando recibido
    if (buffer[0] == 't') {
        // Devolver la hora actual
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[9];  // HH:MM:SS
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        sendto(sockfd, time_str, strlen(time_str), 0, (struct sockaddr *)client_addr, client_len);
    } 
    else if (buffer[0] == 'd') {
        // Devolver la fecha actual
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char date_str[11];  // YYYY-MM-DD
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);
        sendto(sockfd, date_str, strlen(date_str), 0, (struct sockaddr *)client_addr, client_len);
    } 
    else if (buffer[0] == 'q') {
        // Terminar el servidor
        sendto(sockfd, "Saliendo...", 11, 0, (struct sockaddr *)client_addr, client_len);
        printf("Saliendo...\n");
        exit(0);
    } 
    else {
        // Comando no soportado
        // sendto(sockfd, "Comando no soportado", 21, 0, (struct sockaddr *)client_addr, client_len);
        fprintf(stdout, "Comando no soportado: %d\n", buffer[0]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dirección> <puerto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct addrinfo hints, *res;
    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Preparar la estructura hints para getaddrinfo
    hints.ai_family = AF_UNSPEC;       // Aceptar tanto IPv4 como IPv6
    hints.ai_socktype = SOCK_DGRAM;    // UDP
    hints.ai_flags = AI_PASSIVE;       // Para permitir escuchar en cualquier interfaz

    // Obtener la información de la dirección y puerto
    int status = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (status != 0) {
        fprintf(stderr, "Error getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Crear el socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Enlazar el socket con la dirección y puerto especificados
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);  // Ya no necesitamos la estructura de direcciones

    printf("Servidor UDP escuchando en %s:%s\n", argv[1], argv[2]);

    // Bucle para recibir y manejar comandos
    while (1) {
        handle_client(sockfd, &client_addr, client_len);
    }

    // Cerrar el socket al terminar (aunque no se llega a esta parte debido a la salida del servidor en 'q')
    close(sockfd);
    return 0;
}
