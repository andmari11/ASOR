#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/utsname.h>
#include <stdlib.h>


int main(){
    //-rwSr-xr-x 1 root root 64152 abr  9 07:01 /usr/bin/passwd
    printf("%d\n", getuid());
    printf("%d\n", getgid());
    printf("%d\n", geteuid());
    printf("%d\n", getegid());

    return 0;
}