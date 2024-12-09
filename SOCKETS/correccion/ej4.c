#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main (int argc, char * argv[]) {
    if (argc < 3) {
        printf("Formato incorrecto. Introduce una dirección IPv4 o IPv6 y un puerto como comandos\n");
        return -1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // Datagram socket (UDP)
    hints.ai_flags = AI_PASSIVE; // For wildcard IP address

    struct addrinfo *result;
    if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0) {
        perror("getaddrinfo()");
        return -1;
    }

    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (bind(sd, result->ai_addr, result->ai_addrlen) != 0) {
        perror("bind()");
        return -1;
    }
    freeaddrinfo(result);

    char comando[2]; // Buffer para el comando
    char host[NI_MAXHOST]; // Buffer cliente host name
    char serv[NI_MAXSERV]; // Buffer cliente service name

    //variables para recvfrom
    struct sockaddr_storage client_addr; // Client address structure
    socklen_t client_addrlen = sizeof(client_addr); // Length of the client address

    fd_set dflectura; 
    int df = -1; 

    while(1) {
        while (df == -1) {
            FD_ZERO(&dflectura); // Clear the file descriptor set
            FD_SET(sd, &dflectura); // Add the socket descriptor to the set
            FD_SET(0, &dflectura); // Add the standard input (console) to the set
            df = select(sd + 1, &dflectura, NULL, NULL, NULL); // Wait for an activity on the descriptors
        }

        time_t tiempo = time(NULL); 
        struct tm *tm = localtime(&tiempo); 
        char buf[50];

        if (FD_ISSET(sd, &dflectura)) { // Check if there is activity on the socket
            ssize_t c = recvfrom(sd, comando, 2, 0, (struct sockaddr *) &client_addr, &client_addrlen); // Receive the command from the client
            getnameinfo((struct sockaddr *) &client_addr, client_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV); // Get the client's host and service names
            printf("[RED] %i byte(s) de %s:%s\n", (int)c, host, serv); 
            comando[1] = '\0'; 

            char *response = process_command(comando[0], tm);
            sendto(sd, response, strlen(response), 0, (struct sockaddr *) &client_addr, client_addrlen);
        } else { // If there is activity on the console
            read(0, comando, 2); // Read the command from the console
            printf("[Consola] %i byte(s)\n", 2);
            comando[1] = '\0'; 

            char *response = process_command(comando[0], tm);
            printf("%s\n", response); // Print the response
        }
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

    return 1;
}