#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>


int main(){

    time_t t;
    time(&t);
    struct tm *tiempo=localtime(&t);
    char* ret;
    strftime(ret, 255, "%a %b %d %r %Z %Y", tiempo);
    printf("%s",ret);
    return 0;
}