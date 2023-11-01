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
    if (phdr[i].p_type == PT_LOAD)
    {
      void *virt_mem = (void *)(uintptr_t)phdr[i].p_vaddr;
      size_t mem_size = phdr[i].p_memsz;
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
    return;
  }

  unsigned int phdr_offset = ehdr->e_phoff;
  count_phdr_entry = ehdr->e_phnum;
  unsigned int size_phdr_entry = ehdr->e_phentsize;
  unsigned int elfentry = ehdr->e_entry;
  int flag = 1;

  // allocating space for program header
  phdr = (Elf32_Phdr *)malloc(sizeof(Elf32_Phdr) * count_phdr_entry);

  // looping over program header entries
  int counter = 0;
  for (size_t i = 0; i < ehdr->e_phnum; i++)
  {
    if (lseek(fd, phdr_offset + (i * size_phdr_entry), SEEK_SET) == -1)
    {
      close(fd);
      exit(EXIT_FAILURE);
    }
    int k = read(fd, &phdr[i], sizeof(Elf32_Phdr));
    if (k == -1)
    {
      perror("Error in Program_header");
    }
    // if PT_LOAD is found
    if (phdr[i].p_type == PT_LOAD)
    {
      flag = 0;
      counter++;
      // checking if the entry point lies in the segment and only then loading
      // if (phdr[i].p_memsz > (elfentry) - (phdr[i].p_vaddr) && phdr[i].p_vaddr <= elfentry) {
        
      // creating a virtual mapping
      void *virt_mem = mmap((void *)(uintptr_t)phdr[i].p_vaddr, phdr[i].p_memsz, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, phdr[i].p_offset);
      if (virt_mem == MAP_FAILED)
      {
        perror("mmap");
        exit(EXIT_FAILURE);
      }
    }
  }
  if (flag)
  {
    perror("Error PT_LOAD not in elf file");
    return;
  }

  printf("Number of the times the loop executed %d\n", counter);
  // typecasting to a function pointer
  int (*_start)(void) = (int (*)(void))ehdr->e_entry;

  // the last two steps of calling the function and printing the result
  int result = _start();
  printf("User _start return value = %d\n", result);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <elf_filename>\n", argv[0]);
    return 1;
  }

  load_and_run_elf(argv);
  loader_cleanup(); // Cleanup resources
  return 0;
}
