#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define RESERVED_MEMORY_OFFSET  0x0000000090000000
#define RESERVED_MEMORY_SIZE 0xfffff

//see:
//https://elinux.org/Memory_Management#Reserving_.28and_accessing.29_the_top_of_memory_on_startup
//https://stackoverflow.com/questions/31140987/how-do-i-directly-access-reserved-memory-with-a-kernel-module/31146151#31146151



/*
Reserving (and accessing) the top of memory on startup
A quote from Todd's email on how to use the reserved physical memory in "mem=".

Given that you have a fixed address for your memory, and is already reserved, the easier way to use it is by calling mmap() over the /dev/ mem device, use 0 as the start address, and the physical address of the reserved memory as the offset. The flags could be MAP_WRITE| MAP_READ. That will return you a pointer on user space for your memory mapped by the kernel. For example

If your SDRAM base address is 0x80000000 and your memory is of 64MB, but you use the cmdline mem=60M to reserve 4MB at the end. Then your reserved memory will be at 0x83c00000, so all you need to do is

int fd;
char *reserved_memory;
 
fd = open("/dev/mem",O_RDWR);
reserved_memory = (char *) mmap(0,4*1024*1024,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x83c00000);
 */
int main() {
  int fd;
  char *reserved_memory;
  char *buffer = "Hello World!";
  
  fd = open("/dev/mem", O_RDWR | O_SYNC);
  /* Returns a pointer to the 4GB point in /dev/mem - the start of my reserved memory. Only mapping 4096 bytes. */
  reserved_memory = (char *) mmap(0,RESERVED_MEMORY_SIZE , PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, RESERVED_MEMORY_OFFSET);
  if (reserved_memory == MAP_FAILED) {
    printf("Failed to creating mapping.\n");
    printf("ERRNO: %s\n", strerror(errno));
    return -1;
  }
  sprintf(reserved_memory, "%s", buffer);
  return 0;
}
