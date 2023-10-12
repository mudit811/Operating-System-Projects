#include "header.h"
void submit(char* const Argv[],int ncpu,int tslice){
    pid_t status = fork();
    if (status <= -1)
    {
        printf("Child not created\n");
        exit(0);
    }
    else if (status == 0)
    {
        // printf("I am  the child (%d)\n", getpid());
        
    }
    else if (status > 0)
    {
        
      
    }
}