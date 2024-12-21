#include <file.h>
#include <list.h>
#include <search.h>
#include <stdio.h>

char* program = "ret";
char* usage = "ret file";

int main(int argc, char** argv) {
  struct file_info* file;
  Elf64_Ehdr header;
  struct list* segments;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s\n", usage);
    exit(EXIT_FAILURE);
  }

  file = file_open(argv[1]);

  if (file->fd < 0) {
    fprintf(stderr, "%s: %s: No such file or directory\n", program, file->pathname);
    exit(EXIT_FAILURE);
  }

  if (file->buf.addr == MAP_FAILED) {
    fprintf(stderr, "%s: error: mmap failed\n", program);
    exit(EXIT_FAILURE);
  }

  segments = list_alloc(NULL);

  elf_header_get(file, &header);
  elf_segments_alloc(file, &header, &segments, PF_X);

  print_search(segments);

  elf_segments_free(&segments);
  list_free(&segments);
  file_close(file);

  return 0;
}
