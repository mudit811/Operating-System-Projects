#include <stdio.h>

int main() {
    const int LOOP_ITERATIONS = 100000000;  // Adjust this value to control the execution time

    printf("Program running for about 5 seconds...\n");

    // Perform a large number of computations
    for (int i = 0; i < LOOP_ITERATIONS; i++) {
        // Some basic computation
        int result = i * i;
    }

    printf("Program finished.\n");

    return 0;
}



