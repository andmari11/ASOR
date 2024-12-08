#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
# include <sys/sysmacros.h>


int main(){

    char* file="5.c";
    struct stat infoFile;

    stat(file, &infoFile);

    printf("Major: %d\n", major(infoFile.st_dev));
    printf("Major: %d\n", minor(infoFile.st_dev));

    printf("Inodo: %ld\n", infoFile.st_ino);
    printf("Tipo: %d\n", infoFile.st_mode);

    //st_mtime: ultima hora modificacion contenido
    //st_ctime: ultima hora modificacion estado (permisos, propietario)

    return 0;
}