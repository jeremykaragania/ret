#ifndef SEARCH_H
#define SEARCH_H

#include <file.h>
#include <stdio.h>
#include <udis86.h>
#include <unistd.h>

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
  uint64_t base;
};

void print_search(struct search_info* search, struct list* segments);

#endif
