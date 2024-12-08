#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>


int main(){
    pid_t id=getpid();

    struct sched_param parametrosSched;
    if (sched_getparam(0, &parametrosSched) == -1) {
        perror("Error al obtener los parámetros de planificación");
        return 1;
    }

    printf("Prioridad actual del proceso (sched_getparam): %d\n", parametrosSched.sched_priority);

    int niceness = getpriority(PRIO_PROCESS, 0);
    printf("Prioridad actual del proceso (getpriority): %d\n", niceness);


    int policy = sched_getscheduler(0);

    int max_priority = sched_get_priority_max(policy);
    int min_priority = sched_get_priority_min(policy);
    printf("Prioridad máxima para esta política: %d\n", max_priority);
    printf("Prioridad mínima para esta política: %d\n", min_priority);
    return 0;
}