// Ejercicio 2. Escribir un servidor UDP de hora de forma que:
// ● La dirección y el puerto son el primer y segundo argumento del programa. Las direcciones
//   pueden expresarse en cualquier formato (nombre de host, notación de punto...). Además,
//   el servidor debe funcionar con direcciones IPv4 e IPv6 .
// ● El servidor recibirá un comando (codificado en un carácter), de forma que ‘t’ devuelva la
//   hora, ‘d’ devuelve la fecha y ‘q’ termina el proceso servidor.
// ● En cada mensaje el servidor debe imprimir el nombre y puerto del cliente, usar getnameinfo(3)

/*
    1. Se debe inicializar la estructura hints a cero y configurarla (flags, familia y tipo).
    2. Se debe llamar a getaddrinfo para obtener la información de la dirección 
        arg[1] = nombre del host
        arg[2] = puerto
    3. Se debe crear un socket usando la información de dirección obtenida de getaddrinfo (socket(res.family, res.socktype, res.protocol))
    4. Se debe enlazar el socket a la dirección (bind(soscket, res.addr, res.protocol)) y liberar la memoria asignada por getaddrinfo
    5. Se debe recibir un comando del cliente (recvfrom(socket, buffer, size_array, 0, client_addr, client_len))
    6. Se debe convertir la hora actual a hora local (time(NULL), localtime(&t))
    7. Se debe enviar la hora o la fecha al cliente (sendto(socket, ret, size_array, 0, client_addr, client_len))
    
    
*/


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


int main(int argc, char* argv[]){
    if(argc != 3){
        fprintf(stderr, "Formato incorrecto\n", argv[0]);
        return -1;
    }
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;  // Para obtener direcciones locales
    hints.ai_family = AF_UNSPEC;  // No especificamos IPv4 ni IPv6
    hints.ai_socktype = SOCK_DGRAM;  // Buscar solo SOCK_DGRAM por defecto

    // Declarar un puntero a la estructura addrinfo para almacenar el resultado de getaddrinfo
    struct addrinfo *res;

    // Llamar a getaddrinfo para obtener la información de la dirección para el nombre de host y servicio dados
    // Si getaddrinfo falla, imprimir un mensaje de error y devolver -1
    if(getaddrinfo(argv[1], argv[2], &hints, &res) != 0){
        fprintf(stderr, "Error getaddrinfo()\n");
        return -1;
    }

    // Crear un socket usando la información de dirección obtenida de getaddrinfo
    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // Enlazar el socket a la dirección
    // Si bind falla, imprimir un mensaje de error y devolver -1
    if(bind(sd, res->ai_addr, res->ai_addrlen) != 0){
        fprintf(stderr, "Error bind()\n");
        return -1;
    }

    // Liberar la memoria asignada por getaddrinfo
    freeaddrinfo(res);

    // Declarar variables para el comando, host y servicio
    char comando[2];
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    // Declarar una estructura sockaddr_storage para almacenar la dirección del cliente
    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Obtener la hora actual y convertirla a hora local
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // Buffer para almacenar la cadena de fecha/hora formateada
    char ret[50];

    while (1) {
        // Recibir un comando del cliente
        size_t bytes=recvfrom(sd, comando, 2, 0, (struct sockaddr *) &client_addr, &client_len);
        comando[1] = '\0';

        // La función getnameinfo convierte una dirección de socket en una cadena de texto legible.
        // Parámetros:
        // - (struct sockaddr *) &client_addr: Puntero a la estructura sockaddr que contiene la dirección del cliente.
        // - client_len: Longitud de la estructura sockaddr.
        // - host: Buffer donde se almacenará el nombre del host resultante.
        // - NI_MAXHOST: Tamaño máximo del buffer del nombre del host.
        // - serv: Buffer donde se almacenará el nombre del servicio resultante.
        // - NI_MAXSERV: Tamaño máximo del buffer del nombre del servicio.
        // - NI_NUMERICHOST | NI_NUMERICSERV: Flags que indican que se deben devolver las direcciones numéricas en lugar de los nombres de host y servicio.
        getnameinfo((struct sockaddr *) &client_addr, client_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("(%i bytes) Comando recibido de %s:%s\n",bytes, host, serv);

        switch(comando[0]) {
            case 't': {
                size_t bytes = strftime(ret, 49, "%I:%M:%S %p", tm);
                ret[bytes] = '\0';
                sendto(sd, ret, bytes, 0, (struct sockaddr *) &client_addr, client_len);
                break;
            }
            case 'd': {
                size_t bytes = strftime(ret, 49, "%Y-%m-%d", tm);
                ret[bytes] = '\0';
                sendto(sd, ret, bytes, 0, (struct sockaddr *) &client_addr, client_len);
                break;
            }
            case 'q': {
                printf("Terminado el proceso servidor...\n");
                return 1;
            }
            default: {
                printf("Comando no soportado.\n");
                break;
            }
        }
    }    

    return 1;
}
