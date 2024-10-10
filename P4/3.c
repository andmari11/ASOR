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
//./ejec SLEEP_SECS=2

int main(int argc, char *argv[]){

    sigset_t blk_set;

    // Registrar manejadores de señales
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    sigemptyset(&blk_set);
    sigaddset(&blk_set, SIGINT);         /* bloquea Ctrl+C */
    sigaddset(&blk_set, SIGTSTP);        /* bloquea Ctrl+Z */
    sigprocmask(SIG_BLOCK, &blk_set, NULL);

    sleep(atoi(getenv("SLEEP_SECS")));

    sigprocmask(SIG_UNBLOCK, &blk_set, NULL);
    printf("\n");
    if(sigint_received>0)
        printf("se recibió sigint\n");
    if(sigtstp_received>0)
        printf("se recibió sigstp\n");

    return 1;
}