#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(){

    for(int i=0; i<255; i++){

        fprintf(stderr, "%s\n", strerror(i));
    }
    return 1;
}