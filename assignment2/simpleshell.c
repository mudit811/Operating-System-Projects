#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>

char Input[100];
char *Args[20];
bool Background_process = false;
// int historyCount = 0;


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
        printf("Something bad happened\n");
        exit(0);
    }
    else if (status == 0)
    {
        printf("I am the child (%d)\n", getpid());
        parse_command();
        execvp(Args[0], Args);
        perror("Error in executing command");
        exit(EXIT_FAILURE);
    }
    else if (status > 0)
    {
        // Background process
        if (!Background_process)
        {
            wait(NULL);
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
        printf("I am the parent Shell\n");
    }

}


int launch(char *command)
{
    int status;
    status = create_process_and_run(command);
    return status;
}


char * read_user_input()
{

    fgets(Input, sizeof(Input), stdin);

    Input[strlen(Input) - 1] = '\0';
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
        status = launch(command);
    } while (status);
}

int main()
{

    bool runInBackground = false;
    shell_loop();
}