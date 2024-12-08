#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <limits.h>
#include <stdlib.h>



int main(){
    pid_t pid=getpid();
    pid_t ppid=getppid();
    //proceso actual
    pid_t pgid = getpgid(0); 
    pid_t sid = getsid(0);

    printf("PID del proceso: %d\n", pid);
    printf("PPID (PID del padre): %d\n", ppid);
    printf("PGID (ID del grupo de procesos): %d\n", pgid);
    printf("SID (ID de la sesión): %d\n", sid);

    struct rlimit limit;
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("Error al obtener el límite de archivos abiertos");
        return 1;
    }
    printf("Número máximo de ficheros que puede abrir: %ld\n", limit.rlim_cur);

    char cwd[255]; 
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error al obtener el directorio de trabajo");
        return 1;
    }
    printf("Directorio de trabajo actual: %s\n", cwd);


    return 0;
}