#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void print_socket_info(struct addrinfo *res) {
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    int family = res->ai_family;
    int socktype = res->ai_socktype;

    // Obtener el nombre del host en formato numérico
    int err = getnameinfo(res->ai_addr, res->ai_addrlen, host, sizeof(host), service, sizeof(service), NI_NUMERICHOST);
    if (err != 0) {
        fprintf(stderr, "Error en getnameinfo(): %s\n", gai_strerror(err));
        return;
    }

    // Mostrar la IP numérica
    printf("%s\t", host);

    // Mostrar la familia de direcciones
    if (family == AF_INET) {
        printf("2\t");
    } else if (family == AF_INET6) {
        printf("10\t");
    } else {
        printf("Desconocida\t");
    }

    // Mostrar el tipo de socket
    if (socktype == SOCK_STREAM) {
        printf("1\n");  // SOCK_STREAM
    } else if (socktype == SOCK_DGRAM) {
        printf("2\n");  // SOCK_DGRAM
    } else if (socktype == SOCK_RAW) {
        printf("3\n");  // SOCK_RAW
    } else {
        printf("Desconocido\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <host>\n", argv[0]);
        return 1;
    }

    struct addrinfo hints, *res;
    int status;
    hints.ai_flags = AI_PASSIVE;  // Para obtener direcciones locales
    hints.ai_family = AF_UNSPEC;  // No especificamos IPv4 ni IPv6
    hints.ai_socktype = SOCK_STREAM;  // Buscar solo SOCK_STREAM por defecto

    // Obtener todas las posibles direcciones
    status = getaddrinfo(argv[1], NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "Error getaddrinfo(): %s\n", gai_strerror(status));
        return 1;
    }

    // Recorrer todas las direcciones devueltas
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        print_socket_info(p);

        // Buscar para otros tipos de socket (SOCK_DGRAM, SOCK_RAW)
        hints.ai_socktype = SOCK_DGRAM;
        status = getaddrinfo(argv[1], NULL, &hints, &res);
        if (status == 0) {
            print_socket_info(res);
        }

        hints.ai_socktype = SOCK_RAW;
        status = getaddrinfo(argv[1], NULL, &hints, &res);
        if (status == 0) {
            print_socket_info(res);
        }

        hints.ai_socktype = SOCK_STREAM;
        status = getaddrinfo(argv[1], NULL, &hints, &res);
        if (status == 0) {
            print_socket_info(res);
        }
    }

    // Liberar memoria
    freeaddrinfo(res);
    return 0;
}
