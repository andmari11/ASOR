#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <pwd.h>


int main(){

    struct passwd *resultado= getpwuid(getuid());

    printf("Usuario %d: %s - %s\n", getuid(), resultado->pw_name, resultado->pw_gecos);

    return 0;
}