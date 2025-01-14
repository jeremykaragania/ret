#include <file.h>
#include <list.h>
#include <search.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char* program;
char* optstring = ":b:l:";

void usage() {
  char* usagestring = "[-b base] [-l length] elffile";
  fprintf(stderr, "Usage: %s %s\n", program, usagestring);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  struct file_info* file;
  struct search_info search;
  Elf64_Ehdr header;
  struct list* segments;
  int opt;

  program = argv[0];

  if (argc < 2) {
    usage();
  }

  search.has_base = 0;
  search.base = 0;
  search.gadget_length = 2;
  opt = 1;

  while((opt = getopt(argc, argv, optstring)) > 0) {
    switch (opt) {
      case 'b':
        search.has_base = 1;
        search.base = strtoll(optarg, NULL, 16);
        break;
      case 'l':
        search.gadget_length = strtoll(optarg, NULL, 10) + 1;
        break;
      default:
        usage();
    }
  }

  if (optind != argc - 1) {
    usage();
  }

  file = file_open(argv[optind]);

  if (file->fd < 0) {
    fprintf(stderr, "%s: %s: No such file or directory\n", program, file->pathname);
    exit(EXIT_FAILURE);
  }

  if (file->buf.addr == MAP_FAILED) {
    fprintf(stderr, "%s: error: mmap failed\n", program);
    exit(EXIT_FAILURE);
  }

  elf_header_get(file, &header);

  if (!is_elf_ident_valid(&header)) {
    fprintf(stderr, "%s: error: Not an ELF file\n", program);
    exit(EXIT_FAILURE);
  }

  if (!is_elf_machine_valid(&header)) {
    fprintf(stderr, "%s: error: File format not recognized\n", program);
    exit(EXIT_FAILURE);
  }

  segments = list_alloc(NULL);

  elf_segments_alloc(file, &header, &segments, PF_X);

  print_search(&search, segments);

  elf_segments_free(&segments);
  list_free(&segments);

  file_close(file);

  return 0;
}
