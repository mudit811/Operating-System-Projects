## Operating Systems
### Assignment-1

AIM: to build a simple loader

Github repo:
https://github.com/theniceguy8731/OS-assignments.git

#### Contributions:
Mudit Bansal-
Worked majorly on error handling throughout the project and building the launcher as well as makefiles

#### Kushagra Gupta-
Worked majorly on building the loader file and shared library

#### Explainer:

1. **Initialization and Header Inclusion:**
   - I begin by including the necessary "loader.h" header file to access required definitions.
   - Global variables are set up to hold the ELF header (`ehdr`), program header (`phdr`), file descriptor (`fd`), and variables to track the program header entry count and sizes.

2. **Resource Cleanup - `loader_cleanup` Function:**
   - This function manages resource cleanup and memory release.
   - I loop through each program header entry and for segments of type `PT_LOAD`, I unmap the corresponding memory region using `munmap`.
   - After the loop, I free the memory allocated for ELF and program headers, and close the file descriptor.

3. **Loading and Execution - `load_and_run_elf` Function:**
   - The main logic of loading and executing the ELF program resides here.
   - The function takes the ELF filename as a parameter.

4. **Opening the ELF File:**
   - I open the specified ELF file using the given filename.
   - If the file opening fails (returning `-1`), I display an error message and exit the function.

5. **Allocating Memory for ELF Header:**
   - I allocate memory to hold the ELF header and read the ELF header information from the file into this memory space.
   - I extract important values such as the program header offset, program header entry count, entry size, and the ELF entry point.

6. **Reading Program Header Entries:**
   - Memory is allocated to store a program header entry.
   - I read each program header entry sequentially, storing the information in the allocated memory.

7. **Loading Loadable Segments:**
   - For each program header entry, if its type (`p_type`) is `PT_LOAD` (value 1), I set a flag indicating a loadable segment has been found.
   - I check if the ELF entry point lies within the virtual address range of the segment.
   - If conditions are met, I use `mmap` to map a virtual memory region and copy the segment data from the file into the mapped memory.

8. **Calling the ELF Entry Point:**
   - I typecast the entry point (`elfentry`) into a function pointer of the correct type (`int (*)(void)`).
   - Execution of the program begins at this point.
   - I also print the return value of this function.

9. **Error Handling - No Loadable Segment:**
   - If the flag is still set to 1, I display an error message, indicating that no loadable segment was found.

10. **Resource Cleanup and Finalization:**
    - Regardless of the outcome, I ensure proper resource cleanup by calling the `loader_cleanup` function to release memory and close the file descriptor.

11. **Main Function - Command-line Argument Check:**
    - In the `main` function, I verify that the correct number of command-line arguments (only the ELF filename) is provided.
    - If not, I display the correct usage format.

12. **Calling the Loader and Exiting:**
    - If the command-line arguments are correct, I call the `load_and_run_elf` function with the provided ELF filename.
    - I return 0 to indicate successful execution.