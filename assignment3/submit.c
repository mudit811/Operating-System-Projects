#include "header.h"
Process *submit(char *const Argv[], int ncpu, int tslice, Process *p)
{
    pid_t status = fork();
    int flag = 0, fd[2];
    if (pipe(fd) == -1)
    {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    if (status <= -1)
    {
        printf("Child not created\n");
        exit(0);
    }
    else if (status == 0)
    {
        close(fd[1]);
        while (flag == 0)
        {
            read(fd[0], &flag, sizeof(flag));
        }
        close(fd[0]);
        
        execvp(Argv[0],Argv);
        // printf("I am  the child (%d)\n", getpid());
        // exec
    }
    else if (status > 0)
    {
        close(fd[0]);
        int result= kill(status,SIGSTOP);
        flag=1;
        write(fd[1],&flag,sizeof(flag));
        close(fd[1]);
        strcpy(p->executable,Argv);
        p->pid = status;
        return p;
    }
}