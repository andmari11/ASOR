// Ejercicio 1. Escribir un programa que obtenga todas las posibles direcciones con las que se podría
// crear un socket asociado a un host dado como primer argumento del programa. Para cada dirección,
// mostrar la IP numérica, la familia de protocolos y tipo de socket. El programa se implementará
// usando getaddrinfo(3) para obtener la lista de posibles direcciones de socket (struct
// sockaddr). Cada dirección se imprimirá en su valor numérico, usando getnameinfo(3) con el flag
// NI_NUMERICHOST, así como la familia de direcciones y el tipo de socket.

/*
    1. El programa debe recibir un argumento que será el nombre del host.
    2. Se debe inicializar la estructura hints a cero.
    3. Se debe configurar hints (flags, familia de direcciones y tipo de socket).
    4. Se debe llamar a getaddrinfo para obtener la información de direcciones.
    5. Se debe iterar sobre los resultados obtenidos.
    6. Se debe imprimir la dirección IP numérica, la familia de direcciones y el tipo de socket.
    
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char*argv[]){
    // Verifica si el usuario proporcionó un argumento
    if(argc < 2){
        fprintf(stderr, "Formato incorrecto\n", argv[0]);
        return -1;
    }

    // Inicializa la estructura hints a cero
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;  // Para obtener direcciones locales
    hints.ai_family = AF_UNSPEC;  // No especificar IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM;  // Buscar solo SOCK_STREAM por defecto

    struct addrinfo *res;
    // Obtener información de direcciones
    int status = getaddrinfo(argv[1], NULL, &hints, &res);
    if(status != 0){
        fprintf(stderr, "Error getaddrinfo(): %s\n", gai_strerror(status));
        return -1;
    }

    struct addrinfo *iterator;
    // Iterar a través de los resultados
    for(iterator = res; iterator != NULL; iterator = iterator->ai_next){
        switch(iterator->ai_family){
            case AF_INET:
                // Manejar direcciones IPv4
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)iterator->ai_addr;
                char ip4[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(ipv4->sin_addr), ip4, INET_ADDRSTRLEN);
                printf("IPv4: %s\t", ip4);
                break;
            case AF_INET6:
                // Manejar direcciones IPv6
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)iterator->ai_addr;
                char ip6[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip6, INET6_ADDRSTRLEN);
                printf("IPv6: %s\t", ip6);
                break;
            default:
                // Manejar familias de direcciones desconocidas
                printf("Desconocida\t");
                break;
        }
        // Imprimir la familia de direcciones y el tipo de socket
        printf("%i\t%i\t\n", iterator->ai_family, iterator->ai_socktype);
    }
    // Liberar la información de direcciones
    freeaddrinfo(res);
    return 0;
