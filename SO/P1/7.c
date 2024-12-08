#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/utsname.h>
#include <stdlib.h>

int main(){

    printf("%ld\n", pathconf("/home/usuario_vms/Desktop", _PC_LINK_MAX));
    printf("%ld\n", pathconf("/dev/nvme0n1p1", _PC_LINK_MAX));

    return 0;
}