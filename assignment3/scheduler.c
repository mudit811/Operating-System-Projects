#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <regex.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <linux/fcntl.h>
#include <semaphore.h>
#include <time.h>

#define MAX_CMD_LEN 100 // Maximum command length

typedef struct Process
{
    char executable[MAX_CMD_LEN];
    pid_t pid;
    int execution_time;
    int wait_time;
    struct Process *next;
} Process;

typedef struct ReadyQueue
{
    Process *front;
    int ncpu;
    int tslice;
    Process *rear;
    sem_t mutex;
} ReadyQueue;

#define SHM_SIZE sizeof(ReadyQueue)
#define SHM_NAME "ready_queue"

void enqueue(Process *p, ReadyQueue *queue)
{
    if (queue->rear == NULL)
    {
        // Queue is empty, set both front and rear to the new process
        queue->front = queue->rear = p;
    }
    else
    {
        // Add the new process to the end of the queue
        queue->rear->next = p;
        queue->rear = p;
    }
}
Process *dequeue(ReadyQueue *queue)
{
    if (queue->front == NULL)
    {
        return NULL;
    }
    else
    {
        Process *p = queue->front;
        queue->front = queue->front->next;
        return p;
    }
}

int main()
{
    int shm_fd;
    ReadyQueue *queue;
    // Open the shared memory object
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    // Map the shared memory into the address space
    queue = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (queue == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }    
    //initialized the semaphore
    // if (sem_init(&queue->mutex, 1, 1) != 0) {
    //     perror("sem_init");
    //     return EXIT_FAILURE;
    // }
    int ncpu, tslice;
    ncpu = queue->ncpu;
    tslice = queue->tslice;

    clock_t start_time, current_time;
    double elapsed_time;
    int status;
    start_time = clock(); // Initial start time
    Process *process_array[ncpu];
    printf("%d",tslice);

    while (1)
    {
        current_time = clock();                                                       // Get the current time
        //elapsed_time = ((double)(current_time - start_time)) / CLOCKS_PER_SEC * 1000; // Calculate elapsed time in milliseconds

        // Check if the desired interval has passed
        if ((current_time - start_time) >= tslice * CLOCKS_PER_SEC / 1000)
        {
            // fflush(stdout);
            // int a = 0;
            // while (process_array[a]!=NULL){
            //     //pid_t result = waitpid(process_array[a]->pid, &status, WNOHANG) ;

            //     int res=kill(process_array[a]->pid,SIGSTOP);
            //     enqueue(process_array[a],queue);
            //     a++;
            // }
            // while (a < ncpu)
            // {
            //     process_array[a] = dequeue(queue);
            //     if (process_array[a] != NULL)
            //     {
            //         int res = kill(process_array[a]->pid, SIGCONT);
            //     }
            //     else
            //         a = ncpu;
            //     a++;
            // }
            start_time = current_time;
        }
    }
    //sem_destroy(&queue->mutex);
    munmap(SHM_NAME,SHM_SIZE);
    close(shm_fd);
    //puts("bancho5");
    exit(0);
}