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

int main (int argc, char* argv[]){
    if(argc!=4){
        printf("Formato incorrecto\n");
        return -1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *res;
    if(getaddrinfo(argv[1], argv[2], &hints, &res) != 0){
        fprintf(stderr, "Error getaddrinfo()\n");
        return -1;
    }

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(bind(sd, res->ai_addr, res->ai_addrlen) != 0){
        fprintf(stderr, "Error bind()\n");
        return -1;
    }
    freeaddrinfo(res);

    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof(client_addr);

    //enviamos comando
    sendto(sd, argv[3], strlen(argv[3]), 0, res->ai_addr, res->ai_addrlen);
    printf("%s\n", argv[3]);

    //recibimos respuesta
    if(argv[3][0] == 't' || argv[3][0] == 'd'){
        char buffer[80];
        int bytes = recvfrom(sd, buffer, 80, 0, (struct sockaddr *)&client_addr, &client_len);
        buffer[bytes] = '\0';
        printf("%s\n", buffer);
    }
    else{
        printf("Comando no soportado\n");
    }
}