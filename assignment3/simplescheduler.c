#include "header.h"

void scheduler(int ncpu,int tslice){
    int sched_pid=fork();
    if (sched_pid == -1) {
        perror("Fork failed");
        exit(1);
    } 
    else if (sched_pid == 0) {
        // Child process
    } 
    else {
        // Parent process
    }


}