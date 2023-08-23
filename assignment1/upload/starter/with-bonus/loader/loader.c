// hello 

#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;
unsigned int count_phdr_entry;
/*
 * release memory and other cleanups
 */
void loader_cleanup()
{
  for (size_t i = 0; i < count_phdr_entry; i++)
  {
    if (phdr->p_type == PT_LOAD)
    {
      void *virt_mem = (void *)(uintptr_t)phdr->p_vaddr;
      size_t mem_size = phdr->p_memsz;
      if (virt_mem)
      {
        munmap(virt_mem, mem_size);
      }
    }
  }
  free(phdr);
  free(ehdr);

  close(fd);
}

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char **argv)
{
  // opening file
  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
  {
    perror("Error opening File");
    return;
  }

  // allocating space for elf header
  ehdr = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));
  int x = read(fd, ehdr, sizeof(Elf32_Ehdr));
  if (x == -1)
  {
    perror("Error in Elf_header");
  }
  unsigned int phdr_offset = ehdr->e_phoff;
  count_phdr_entry = ehdr->e_phnum;
  unsigned int size_phdr_entry = ehdr->e_phentsize;
  unsigned int elfentry = ehdr->e_entry;
  int flag=1;
  // allocating space for program header
  phdr = (Elf32_Phdr *)malloc(sizeof(Elf32_Phdr));

  // looping over program header entries
  for (size_t i = 0; i < count_phdr_entry; i++)
  {
    lseek(fd, phdr_offset + (i * size_phdr_entry), SEEK_SET);
    int k = read(fd, phdr, sizeof(Elf32_Phdr));
    if (k == -1)
    {
      perror("Error in Program_header");
    }
    // if PT_LOAD is found
    if (phdr->p_type == 1)
    {
      flag=0;
      // checking in the entry point lies in the segment and only then loading
      if (phdr->p_memsz > (elfentry) - (phdr->p_vaddr) && phdr->p_vaddr < elfentry)
      {
        // creating a virtual mapping
        void *virt_mem = mmap((void *)(uintptr_t)phdr->p_vaddr, phdr->p_memsz, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, 0);
        // going to the start of the segment
        lseek(fd, phdr->p_offset, SEEK_SET);
        read(fd, virt_mem, phdr->p_memsz);
      }
      else{
        perror("Error entry point not in PT_LOAD");
      }
    }
  }
  if (flag){
    perror("Error PT_LOAD not in elf file");
  }
  // typecasting to a function pointer
  int (*_start)(void) = (int (*)(void))elfentry;

  // the last two step of calling function and printing result
  int result = _start();
  printf("User _start return value = %d\n", result);
  // 1. Load entire binary content into the memory from the ELF file.
  // 2. Iterate through the PHDR table and find the section of PT_LOAD
  //    type that contains the address of the entrypoint method in fib.c
  // 3. Allocate memory of the size "p_memsz" using mmap function
  //    and then copy the segment content
  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step
  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.
  // 6. Call the "_start" method and print the value returned from the "_start"
}
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <elf_filename>\n", argv[0]);
    return 1;
  }

  load_and_run_elf(argv);
  loader_cleanup();
  return 0;
}