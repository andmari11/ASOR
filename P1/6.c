#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/utsname.h>
#include <stdlib.h>

int main(){

    long resultado= sysconf(_SC_ARG_MAX);
    printf("%ld", resultado);


    return 1;
}