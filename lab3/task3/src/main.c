#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

#define PAGEMAP_ENTRY 8

//X - number
//Y - pos of BIT
#define GET_BIT(X,Y) (X & ((uint64_t)1<<Y)) >> Y

#define ERROR -1

const int __endian_bit = 1;
#define is_bigendian() ( (*(char*)&__endian_bit) == 0 )

int read_pagemap(char * path_buf, unsigned long virt_addr){
   printf("Big endian? %d\n", is_bigendian());
   FILE * f = fopen(path_buf, "rb");
   if(f == NULL){
      printf("Error! Cannot open %s\n", path_buf);
      return -1;
   }
   
   //Shifting by virt-addr-offset number of bytes
   //and multiplying by the size of an address (the size of an entry in pagemap file)
   uint64_t file_offset = virt_addr / getpagesize() * PAGEMAP_ENTRY;
   printf("Vaddr: 0x%lx, Page_size: %d, Entry_size: %d\n", virt_addr, getpagesize(), PAGEMAP_ENTRY);
   printf("Reading %s at 0x%llx\n", path_buf, (unsigned long long) file_offset);
   int status = fseek(f, file_offset, SEEK_SET);
   if(status){
      perror("Failed to do fseek!");
      return -1;
   }

   uint64_t read_val = 0;
   unsigned char c_buf[PAGEMAP_ENTRY];
   for(int i = 0; i < PAGEMAP_ENTRY; ++i){
       int c = getc(f);
       if(c == EOF){
           printf("\nReached end of the file\n");
           fclose(f);
           return 0;
       }
       if(is_bigendian())
           c_buf[i] = c;
       else
           c_buf[PAGEMAP_ENTRY - i - 1] = c;

       printf("[%d]0x%x ", i, c);
    }
    for(int i=0; i < PAGEMAP_ENTRY; ++i){
        read_val = (read_val << 8) + c_buf[i];
    }
    printf("\n");
    printf("Result: 0x%llx\n", (unsigned long long) read_val);
  
    if(GET_BIT(read_val, 62))
        printf("Page swapped\n");
    fclose(f);
    return 0;
}

int main(int argc, char ** argv){
   if(argc!=3){
      printf("Argument number is not correct!\n PID VIRTUAL_ADDRESS\n");
      return ERROR;
   }
   int pid;
   char path_buf [0x100] = {};
   char *end;
   if(!memcmp(argv[1], "self",sizeof("self"))){
      sprintf(path_buf, "/proc/self/pagemap");
      pid = -1;
   }
   else{
         pid = strtol(argv[1], &end, 10);
         if (end == argv[1] || *end != '\0' || pid<=0){ 
            printf("PID must be a positive number or 'self'\n");
            return ERROR;
         }
       }
   unsigned long virt_addr = strtol(argv[2], NULL, 16);
   if(pid != -1)
      sprintf(path_buf, "/proc/%u/pagemap", pid);
   
   read_pagemap(path_buf, virt_addr);
   return 0;
}
