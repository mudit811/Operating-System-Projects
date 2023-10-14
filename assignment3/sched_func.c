#include "header.h"

//###########################
// Kushagra humein kisi tarah se ncpu and tslice bhi scheduler.c ko bhejna hai
// so uske liye maine ready queue mein hi do fields daal di for the same
//#############################
void scheduler(int ncpu,int tslice){
    int sched_pid=fork();
    if (sched_pid == -1) {
        perror("Fork failed");
        exit(1);
    } 
    else if (sched_pid == 0) {
        // Child process
        char *args[]={"./scheduler.out",NULL};
        execv("./scheduler.out",args);
        perror("execv error");
        exit(EXIT_FAILURE);
    } 
    else {
        // Parent process
        // wait(NULL);
        // char * exec_args[]={"./sched_exec",NULL};
        // if (execvp(exec_args[0],exec_args)==-1){
        //     perror("compiled file coudn't be executed");
        //     exit(EXIT_FAILURE);
        // }
    }


}