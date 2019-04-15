#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int test()
{
  int i=0;
  char ch=0;
  int fd=open("one.filter",O_WRONLY | O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  for(i=0; i<(1<<21);++i)
  {
    write(fd,&ch,1);
  }
  close(fd);

  fd=open("two.filter",O_WRONLY | O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  for(i=0; i<(1<<21);++i)
  {
    write(fd,&ch,1);
  }
  close(fd);

  fd=open("three.filter",O_WRONLY | O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  for(i=0; i<(1<<21);++i)
  {
    write(fd,&ch,1);
  }
  close(fd);



  return 0;
}

char* load(char* path)
{
  int fd=open(path,O_RDWR);
  char* addr=mmap(NULL,1<<21,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

  if(addr==MAP_FAILED)
  {
    printf("mmap failed\n");
  }
  char ch=addr[0];
  if(ch)
  {
    printf("ft[0] not null\n");
    addr[0]=0;
  }
  else
  {
    printf("ft[0] null\n");
    addr[0]=1;
  }

  msync(addr,1<<21,MS_SYNC);
  munmap(addr,1<<21);
  close(fd);
  return NULL;
}

char* set(char* path,int offset)
{
  static unsigned char flags[8]={128,64,32,16,8,4,2,1};
  int fd=open(path,O_RDWR);
  unsigned char* addr=mmap(NULL,1<<21,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

  if(addr==MAP_FAILED)
  {
    printf("mmap failed\n");
  }

  addr[offset/8] |= flags[offset%8];
  msync(addr,1<<21,MS_SYNC);
  munmap(addr,1<<21);
  close(fd);
  return NULL;
}

bool get(char* path,int offset)
{
  static unsigned char flags[8]={128,64,32,16,8,4,2,1};
  int fd=open(path,O_RDWR);
  unsigned char* addr=mmap(NULL,1<<21,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

  if(addr==MAP_FAILED)
  {
    printf("mmap failed\n");
  }

  int bit = addr[offset/8] & flags[offset%8];
  
  munmap(addr,1<<21);
  close(fd);

  return bit;
}
