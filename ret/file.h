#ifndef FILE_H
#define FILE_H

#include <elf.h>
#include <fcntl.h>
#include <list.h>
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
  struct segment_info represents ELF file segment information.
*/
struct segment_info {
  uint64_t addr;
  struct buffer_info buf;
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

Elf64_Ehdr* elf_header_get(struct file_info* file, Elf64_Ehdr* ehdr);
struct list* elf_segments_alloc(struct file_info* file, const Elf64_Ehdr* ehdr, struct list** segments, int flags);
void elf_segments_free(struct list** segments);

#endif
