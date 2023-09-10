#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <regex.h>

char Input[256];
char *Args[16];
char *history_book[20];
int historycount = 0;

pid_t backgroundProcesses[100]; 
int backgroundCount=1;
bool back_proc = false;


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
        execvp(Args[0], Args);
        perror("Error in executing command");
        exit(EXIT_FAILURE);
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
            backgroundProcesses[backgroundCount]=getpid();
            printf("[%d]  %d\n", backgroundCount,getpid());
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
int is_valid_format(const char *input) {
    // Define the regular expression pattern
    const char *pattern = "^\\./[^/]+\\.sh$";

    // Compile the regular expression
    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);

    if (ret != 0) {
        fprintf(stderr, "Failed to compile regular expression\n");
        return 0; // Pattern compilation failed
    }

    // Execute the regular expression
    ret = regexec(&regex, input, 0, NULL, 0);

    regfree(&regex);

    return (ret == 0); // Return 1 if the input matches the pattern, 0 otherwise
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

     if (is_valid_format(Input)) {
        // Execute the chmod command
        if (chmod(Input + 2, S_IRUSR | S_IXUSR) == 0) {
        } else {
            perror("chmod");
        }
     
    }
    return Input;
}
// running shell infinite loop
void shell_loop()
{
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
        for (int i = 0; i < backgroundCount; i++) {
            pid_t result = waitpid(backgroundProcesses[i], NULL, WNOHANG);
            if (result > 0) {
                printf("[%d]  %d   Completed\n", i,backgroundProcesses[i]);
                // Background process has completed, remove it from the list
                for (int j = i; j < backgroundCount - 1; j++) {
                    backgroundProcesses[j] = backgroundProcesses[j + 1];
                }
                backgroundCount--;
                i--; 
            }
        }
    } while (status);
}
// this is the main running loop
int main()
{
    bool runInBackground = false;
    // the main shell loop is called
    shell_loop();

}