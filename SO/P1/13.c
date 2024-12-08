#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>


int main(){

    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    for(int i=0;i<1000000000; i++){}
    gettimeofday(&tv2, NULL);

    printf("%ld microsegundos\n", tv2.tv_usec-tv1.tv_usec);
    return 0;
}