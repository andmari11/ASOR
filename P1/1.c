#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(){
    if(setuid(2)==-1){
        perror("Error al cambiar id");
    }
    
    printf("Cambiado con exito");
    return 1;
}