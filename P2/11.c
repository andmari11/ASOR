#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(){

    char* fileName="5.c";
    struct stat infoFile;
    stat(fileName, &infoFile);



    if(S_ISREG(infoFile.st_mode)){
        char symlink_n[255];
        char fixLink[255];

        snprintf(symlink_n, sizeof(symlink_n), "%s.syn", fileName);
        snprintf(fixLink, sizeof(fixLink), "%s.hard", fileName);

        link(fileName, fixLink);
        symlink(fileName, symlink_n);

    }

    return 0;

    
}