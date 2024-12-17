#include <file.h>

/*
  file_open tries opens the file specified by pathname and map the file into
  memory.
*/
struct file_info file_open(char* pathname) {
  struct file_info ret;
  struct stat sb;

  ret.pathname = pathname;
  ret.fd = open(pathname, O_RDONLY);
  fstat(ret.fd, &sb);
  ret.size = sb.st_size;
  ret.addr = mmap(NULL, ret.size, PROT_READ, MAP_PRIVATE, ret.fd, 0);

  return ret;
}

/*
  file_close closes the file specified by "file" and unmaps the file from
  memory.
*/
void file_close(struct file_info* file) {
  close(file->fd);
  munmap(file->addr, file->size);
}
