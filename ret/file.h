#ifndef FILE_H
#define FILE_H

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

/*
  struct buffer represents buffer information.
*/
struct buffer_info {
  void* addr;
  size_t size;
};

/*
  struct file_info represents file information.
*/
struct file_info {
  char* pathname;
  int fd;
  struct buffer_info buf;
};

struct file_info* file_open(char* pathname);
void file_close(struct file_info* file);

#endif
