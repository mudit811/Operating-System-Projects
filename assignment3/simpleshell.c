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

ReadyQueue *queue;

char Input[256];
char *Args[16];
char *history_book[20];
int historycount = 0;
pid_t backgroundProcesses[100];
int backgroundCount = 0;
bool back_proc = false;
int ncpu, tslice;

void scheduler(int ncpu, int tslice)
{
    pid_t sched_pid = fork();
    if (sched_pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (sched_pid == 0)
    {

        char *args[] = {"./scheduler.out", NULL};

        execvp("./scheduler.out", args);
        printf("hello");
        fflush(stdout);
        perror("execv error");
        exit(EXIT_FAILURE);
    }
    else
    {

        // Parent process
        // wait(NULL);
        // char * exec_args[]={"./sched_exec",NULL};
        // if (execvp(exec_args[0],exec_args)==-1){
        //     perror("compiled file coudn't be executed");
        //     exit(EXIT_FAILURE);
        // }
    }
}

Process *submit(char *const Argv[], int ncpu, int tslice, Process *p)
{
    pid_t status = fork();

    // int flag = 0, fd[2];
    //  if (pipe(fd) == -1){
    //      perror("Pipe creation failed");
    //      exit(EXIT_FAILURE);
    //  }
    if (status < 0)
    {
        printf("Child not created\n");
        exit(0);
    }
    else if (status == 0)
    {
        kill(getpid(), SIGSTOP);
        // close(fd[1]);
        // while (flag == 0)
        // {
        //     read(fd[0], &flag, sizeof(flag));
        // }
        // close(fd[0]);
        execvp(Argv[0], Argv);
        // printf("I am  the child (%d)\n", getpid());
    }
    else if (status > 0)
    {
        // close(fd[0]);
        // int result= kill(status,SIGSTOP);
        // flag=1;
        // write(fd[1],&flag,sizeof(flag));
        // close(fd[1]);
        strcpy(p->executable, Argv[0]);
        p->pid = status;
        // printf("%d",p->pid);
        return p;
    }
}

void enqueue(Process *p)
{
    if (queue->front == NULL)
    {
        // Queue is empty, set both front and rear to the new process
        queue->front = queue->rear = p;
    }
    else
    {
        // Add the new process to the end of the queue
        queue->rear->next = p;
        queue->rear = queue->rear->next;
    }
}

int shm_setup()
{
    // Create a shared memory segment
    int shm_fd = shm_open("/ready_queue", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    // Set the size of the shared memory segment
    if (ftruncate(shm_fd, sizeof(ReadyQueue)) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    // Map the shared memory segment into the address space
    queue = (ReadyQueue *)mmap(NULL, sizeof(ReadyQueue), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (queue == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize the shared queue
    queue->front = NULL;
    queue->rear = NULL;

    // Access the shared queue
    return shm_fd;
}

void shm_cleanup(int shm_fd)
{
    // Unmap the shared memory segment
    if (munmap(queue, sizeof(ReadyQueue)) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    // Close the shared memory file descriptor
    if (close(shm_fd) == -1)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }
    // Unlink the shared memory segment
    if (shm_unlink("/ready_queue") == -1)
    {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
    sem_destroy(&queue->mutex);
}

int secure_strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return 1;
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0') ? 0 : 1;
}

// breaks the input command into function and it's arguments by using strtok
void parse_command()
{
    // The  strtok()  function  breaks a string into a sequence of zero or more nonempty tokens.
    char *token = strtok(Input, " ");
    // error checking for strtok
    if (token == NULL)
    {
        exit(EXIT_FAILURE);
    }
    //
    int i = 0;
    while (token != NULL)
    {
        Args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    Args[i] = NULL;
}
// the final function which will run the command
int create_process_and_run(char *command)
{
    pid_t status = fork();
    if (status <= -1)
    {
        printf("Child not created\n");
        exit(0);
    }
    else if (status == 0)
    {
        // printf("I am the child (%d)\n", getpid());
        // parse_command();
        if (back_proc)
        {
            setpgid(0, 0);
        }

        // if the command is of type submit
        if (!secure_strcmp(Args[0], "submit"))
        {
            Process *p = (Process *)malloc(sizeof(Process));
            p = submit(Args, ncpu, tslice, p);
            // sem_wait(&queue->mutex);
            enqueue(p);
            // sem_post(&queue->mutex);
            printf("%d\n", queue->front->pid);
            printf("%d\n", queue->rear->pid);
        }
        else
        {
            execvp(Args[0], Args);
            perror("Error in executing command");
            exit(EXIT_FAILURE);
        }
    }
    else if (status > 0)
    {
        // Background process
        if (back_proc == 0)
        {
            waitpid(status, NULL, 0);
        }
        else
        {
            backgroundProcesses[backgroundCount] = getpid();
            printf("[%d]  %d\n", backgroundCount, getpid());
            backgroundCount++;
        }
    }
    else
    {
        int ret;
        int pid = wait(&ret);
        if (WIFEXITED(ret))
        {
            printf("% d Exit = % d\n", pid, WEXITSTATUS(ret));
        }
        else
        {
            printf("Abnormal termination of % d\n", pid);
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        // printf("I am the parent Shell\n");
    }
}
// the function used to transition from taking input and processing it to running the command
int launch(char *command)
{
    int status;

    parse_command();
    status = create_process_and_run(command);
    return status;
}

int is_valid_format(const char *input)
{
    // Check if the input starts with "./" and ends with ".sh"
    if (strncmp(input, "./", 2) == 0 && strstr(input, ".sh") != NULL)
    {
        // Ensure that there are no '/' characters in the middle
        const char *slash = strchr(input + 2, '/');
        if (slash == NULL || strstr(slash, ".sh") != NULL)
        {
            return 1; // Valid format
        }
    }
    return 0; // Not a valid format
}

// used to take input of a function
char *read_user_input()
{
    // error checking while taking input
    if (fgets(Input, 256, stdin) == NULL)
    {
        perror("Failed to get input");
        exit(EXIT_FAILURE);
    }
    // first it removes the newline character
    if (strlen(Input) > 0 && Input[strlen(Input) - 1] == '\n')
    {
        Input[strlen(Input) - 1] = '\0';
    }
    // then it check if there is an ampersand or not
    if (Input[strlen(Input) - 1] == '&')
    {
        back_proc = true;
        Input[strlen(Input) - 1] = '\0'; // Remove the "&" symbol
    }
    // if there is no ampersand that means it is not a background process
    else
    {
        back_proc = false;
    }

    if (is_valid_format(Input))
    {
        // Execute the chmod command
        if (chmod(Input + 2, S_IRUSR | S_IXUSR) == 0)
        {
        }
        else
        {
            perror("chmod");
        }
    }
    return Input;
}
// running shell infinite loop
void shell_loop()
{
    queue->ncpu = ncpu;
    queue->tslice = tslice;
    scheduler(ncpu, tslice);
    int status;
    do
    {
        printf("$: ");
        // command points to the value returned from read_user_input
        char *command = read_user_input();
        // check if command is empty
        if (!secure_strcmp("", command))
        {
            continue;
        }
        // check in command is exit
        if (!secure_strcmp("exit", command))
        {
            break;
        }
        // check if the command wants history
        if (secure_strcmp(command, "history") == 0)
        {
            for (int i = 0; i < historycount; i++)
            {
                printf("%d: %s\n", i + 1, history_book[i]);
            }
        }
        else
        {
            // store command in history
            history_book[historycount] = strdup(command);
            historycount++;
            status = launch(command);
        }
    } while (status);
}
// this is the main running loop
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <ncpu> <tslice>\n", argv[0]);
        return 1;
    }
    else
    {
        ncpu = atoi(argv[1]);
        tslice = atoi(argv[2]);

        bool runInBackground = false;
        // the main shell loop is called
        int shm_fd = shm_setup();
        // initialized the semaphore
        if (sem_init(&queue->mutex, 1, 1) != 0)
        {
            perror("sem_init");
            return EXIT_FAILURE;
        }
        shell_loop();
        shm_cleanup(shm_fd);
    }
}