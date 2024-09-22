#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>


int main(){

    time_t ret;
    time(&ret);
    printf("%ld\n", ret);

    return 0;
}