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
    if(argc < 2){
        fprintf(stderr, "Formato incorrecto\n", argv[0]);
        return -1;
    }
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;  // Para obtener direcciones locales
    hints.ai_family = AF_UNSPEC;  // No especificamos IPv4 ni IPv6
    hints.ai_socktype = SOCK_STREAM;  // Buscar solo SOCK_STREAM por defecto

    struct addrinfo *res;
    int status = getaddrinfo(argv[1], NULL, &hints, &res);
    if(status != 0){
        fprintf(stderr, "Error getaddrinfo(): %s\n", gai_strerror(status));
        return -1;
    }

    struct addrinfo *iterator;
    for(iterator = res; iterator != NULL; iterator = iterator->ai_next){
        switch(iterator->ai_family){
            case AF_INET:
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)iterator->ai_addr;
                char ip4[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(ipv4->sin_addr), ip4, INET_ADDRSTRLEN);
                printf("IPv4: %s\t", ip4);
                break;
            case AF_INET6:
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)iterator->ai_addr;
                char ip6[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip6, INET6_ADDRSTRLEN);
                printf("IPv6: %s\t", ip6);
                break;
            default:
                printf("Desconocida\t");
                break;
        }
        printf("%i\t%i\t\n", iterator->ai_family, iterator->ai_socktype);
    }
    freeaddrinfo(res);
    return 0;
}