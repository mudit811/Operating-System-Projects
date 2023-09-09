#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 256
#define MAX_HISTORY_SIZE 10
#define MAx_ARGUMENT_SIZE 16

char Input[MAX_INPUT_SIZE];
char *Args[MAx_ARGUMENT_SIZE];
bool Background_process = false;
char *history_book[MAX_HISTORY_SIZE];
int historycount=0;
void parse_command()
{
    char *token = strtok(Input, " ");
    int i = 0;
    while (token != NULL)
    {
        Args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    Args[i] = NULL;
}

int create_process_and_run(char *command)
{
    pid_t status = fork();
    if (status < 0)
    {
        printf("Child not created\n");
        exit(0);
    }
    else if (status == 0)
    {
        // printf("I am the child (%d)\n", getpid());
        // parse_command();
        execvp(Args[0], Args);
        perror("Error in executing command");
        exit(EXIT_FAILURE);
    }
    else if (status > 0)
    {
        // Background process
        if (!Background_process)
        {
            waitpid(status, NULL, 0);
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

int launch(char *command)
{
    int status;
    parse_command();
    status = create_process_and_run(command);
    return status;
}

char *read_user_input()
{

    if (fgets(Input, MAX_INPUT_SIZE, stdin) == NULL)
    {
        perror("Failed to get input");
        exit(EXIT_FAILURE);
    }

    if (strlen(Input) > 0 && Input[strlen(Input) - 1] == '\n')
    {
        Input[strlen(Input) - 1] = '\0';
    }

    if (Input[strlen(Input) - 1] == '&')
    {
        Background_process = true;
        Input[strlen(Input) - 1] = '\0'; // Remove the "&" symbol
    }

    else
    {
        Background_process = false;
    }
    return Input;
}

void shell_loop()
{
    int status;
    do
    {
        printf("iiitd@possum:~$ ");
        char *command = read_user_input();

        if (!strcmp("", command))
            continue;

        if (!strcmp("exit", command))
            break;
        if (!strcmp(command, "history"))
        {
            // Display history
            for (int i = 0; i < historycount; i++)
            {
                printf("%d: %s\n", i + 1, history_book[i]);
            }
        }
        else
        {
            // Store command in history
            history_book[historycount] = strdup(command);
            historycount++;
            status = launch(command);
        }
    } while (status);
}

int main()
{

    bool runInBackground = false;
    shell_loop();
}