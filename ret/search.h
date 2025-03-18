#ifndef SEARCH_H
#define SEARCH_H

#include <file.h>
#include <list.h>
#include <stdio.h>
#include <string.h>
#include <udis86.h>
#include <unistd.h>

#define INVALID_MNEMONICS_SIZE 2

/*
  instruction_info represents instruction information.
*/
struct instruction_info {
  int mnemonic;
  uint64_t off;
  char buf[128];
};

/*
  search_info represents search information. It is specified by a base address
  "base".
*/
struct search_info {
  struct list* segments;
  int has_base;
  uint64_t base;
  size_t gadget_length;
};

void print_search(struct search_info* search);

#endif
