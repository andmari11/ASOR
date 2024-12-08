#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(){

    umask(027);
    int file=open("fichero.txt", O_CREAT, 0666) ;
    close(file);
    return 0;
}