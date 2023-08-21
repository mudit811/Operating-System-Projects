#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;

/*
 * release memory and other cleanups
 */
void loader_cleanup() {
  
}

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char** argv) {
  fd = open(argv[1], O_RDONLY);
  if(fd==-1){
    perror("Error opening File");
    return;
  }
  ehdr=(Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr));
  int x=read(fd,ehdr,sizeof(Elf32_Ehdr));
  unsigned int phdr_offset=ehdr->e_phoff;
  unsigned int count_phdr_entry=ehdr->e_phnum;
  unsigned int size_phdr_entry=ehdr->e_phentsize;
  phdr=(Elf32_Phdr*)malloc(sizeof(Elf32_Phdr));
  
  for(size_t i=0;i<count_phdr_entry;i++){
    lseek(fd,phdr_offset+(i*size_phdr_entry),SEEK_SET);    
    int k=read(fd,phdr,sizeof(Elf32_Phdr));
    if(phdr->p_type==1){
      printf("%u\n",phdr->p_type);
    }
  }
  // 1. Load entire binary content into the memory from the ELF file.
  // 2. Iterate through the PHDR table and find the section of PT_LOAD 
  //    type that contains the address of the entrypoint method in fib.c
  // 3. Allocate memory of the size "p_memsz" using mmap function 
  //    and then copy the segment content
  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step
  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.
  // 6. Call the "_start" method and print the value returned from the "_start"
  //int result = _start();
  //printf("User _start return value = %d\n",result);
}
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <elf_filename>\n", argv[0]);
        return 1;
    }

    load_and_run_elf(argv);
    return 0;
}