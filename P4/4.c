#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int sigint_received = 0;  // Bandera para SIGINT
int sigtstp_received = 0; // Bandera para SIGTSTP


void signal_handler(int sig) {
    if (sig == SIGINT) {
        sigint_received = sigint_received + 1;  // Indica que se recibió SIGINT
    } else if (sig == SIGTSTP) {
        sigtstp_received = sigtstp_received + 1; // Indica que se recibió SIGTSTP
    }

}

int main(){

    sigset_t blk_set;

    // Registrar manejadores de señales
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    while(sigint_received+sigtstp_received<10){

    }

    printf("\n");
    if(sigint_received>0)
        printf("se recibió %d sigint\n", sigint_received);
    if(sigtstp_received>0)
        printf("se recibió %d sigstp\n", sigtstp_received);

    return 1;
}