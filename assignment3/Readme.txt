Custom Shell - Code Explanation

This custom shell is a basic command-line interpreter written in C. It provides functionality for running commands, managing background processes, and scheduling jobs. The code is divided into various sections, each with specific responsibilities.

1. Data Structures:
   - The code defines two main data structures: `Process` and `ReadyQueue`. 
   - `Process` represents an individual process with attributes such as the executable command, process ID (PID), execution time, and wait time.
   - `ReadyQueue` is a data structure used for process scheduling. It includes a queue of processes, front and rear pointers, the number of CPUs, time slice, and a mutex for synchronization.

2. Queue Management:
   - Functions like `enqueue` and `dequeue` are implemented for managing the ready queue.
   - `queue_size` calculates the size of the queue.

3. Scheduling:
   - The `scheduler` function is responsible for scheduling processes based on the number of CPUs and time slice.
   - It continuously controls the execution and suspension of processes.

4. Job Submission:
   - The `submit` function takes a command and submits it as a job.
   - The command is executed in a child process, and process information is added to the ready queue.

5. Shared Memory:
   - Shared memory is used for inter-process communication.
   - `shm_setup` creates and initializes the shared memory.
   - `shm_cleanup` releases shared memory resources.

6. Input Parsing:
   - User input is read and parsed to extract command and arguments.
   - The `read_user_input` function reads user input and handles background processes.

7. Main Loop:
   - The `shell_loop` function contains the main loop for the shell.
   - It reads and processes user commands, including submitting jobs and managing background processes.

8. Launching:
   - The `create_process_and_run` function creates child processes to execute commands.
   - It can also handle background execution.

9. Execution:
   - The main function `main` initializes the shell, sets up shared memory, and enters the shell loop.
   - Users specify the number of CPUs and time slice as command-line arguments.

The code is well-documented and contains features for basic command execution and process management. Users can submit jobs, run them in the background, and view command history.
