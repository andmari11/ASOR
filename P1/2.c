#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(){

    if(setuid(0)==-1){
        fprintf(stderr, "Error(%d) al cambiar id: %s", errno, strerror(errno));
        exit(-1);
    }
    
    printf("Cambiado con exito");
    return 1;
}