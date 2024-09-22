#define _GNU_SOURCE 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/utsname.h>
#include <stdlib.h>

int main(){
    struct utsname *resultado;
    uname(resultado);
    fprintf(stdout, "%s\n", resultado->domainname);
    fprintf(stdout, "%s\n", resultado->machine);
    fprintf(stdout, "%s\n", resultado->release);
    fprintf(stdout, "%s\n", resultado->nodename);
    fprintf(stdout, "%s\n", resultado->version);
    fprintf(stdout, "%s\n", resultado->sysname);

    return 1;
}