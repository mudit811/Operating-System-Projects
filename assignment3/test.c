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

// #include <stdio.h>

// int main() {
//     const long LOOP_ITERATIONS = 1000000000;  // Adjust this value to control the execution time

//     printf("Program running for about 5 seconds...\n");

//     // Perform a large number of computations
//     for (int i = 0; i < LOOP_ITERATIONS; i++) {
//         // Some basic computation
//         int result = i * i;
//     }

//     printf("Program finished.\n");

//     return 0;
// }
  


int main() {
//     int data_to_send = 42; // Replace with the integer you want to send
//     pid_t status;
//     status=fork();
//     int data=0;
//     if (status==1){
//          kill(getppid(), SIGUSR1);

//     // Sleep for a moment to allow the receiver to catch the signal
//     sleep(1);

//     // Send the integer as the signal's value
//     kill(getppid(), SIGUSR1);
//     sleep(1);
//     }
//     void signalHandler(int signo, siginfo_t* info, void* context) {
//     if (signo == SIGUSR1) {
//         received_data = info->si_value.sival_int;
//         printf("Received data: %d\n", received_data);
//     }
// }
//     // Send the integer using a signal (e.g., SIGUSR1)
   

//     return 0;
   unsigned long int a=0;
while(a<1000){
    a++;
}
printf("hello i am a file");
}