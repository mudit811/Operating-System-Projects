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

#define MAX_CMD_LEN 100  // Maximum command length

typedef struct Process {
    char executable[MAX_CMD_LEN];
    pid_t pid;
    int execution_time;
    int wait_time;
    struct Process *next;
} Process;

typedef struct ReadyQueue {
    Process *front;
    int ncpu;
    int tslice;
    Process *rear;
} ReadyQueue;


void scheduler(int ncpu,int tslice);
Process* submit(char* const argv[],int ncpu,int tslice,Process * process);
ReadyQueue *queue;
