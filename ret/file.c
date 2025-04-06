#include <file.h>

/*
  file_open tries opens the file specified by pathname and map the file into
  memory.
*/
struct file_info* file_open(char* pathname) {
  struct file_info* ret;
  struct stat sb;

  ret = malloc(sizeof(struct file_info));
  ret->pathname = pathname;
  ret->fd = open(pathname, O_RDONLY);

  if (fstat(ret->fd, &sb) < 0) {
    ret->pathname = NULL;
  }

  ret->buf.size = sb.st_size;
  ret->buf.addr = mmap(NULL, ret->buf.size, PROT_READ, MAP_PRIVATE, ret->fd, 0);

  return ret;
}

/*
  file_close closes the file specified by "file" and unmaps the file from
  memory.
*/
void file_close(struct file_info* file) {
  close(file->fd);
  munmap(file->buf.addr, file->buf.size);
  file->fd = -1;
  free(file);
}

/*
  elf_header_get returns the ELF header from the file specified by "file"
  through "ehdr".
*/
Elf64_Ehdr* elf_header_get(struct file_info* file, Elf64_Ehdr* ehdr) {
  *ehdr = *(Elf64_Ehdr*)file->buf.addr;
  return ehdr;
}

/*
  is_elf_ident_valid returns 1 if the identification specified by the header
  "ehdr" is valid and 0 if it isn't.
*/
int is_elf_ident_valid(Elf64_Ehdr* ehdr) {
  return ehdr->e_ident[EI_MAG0] == ELFMAG0 && ehdr->e_ident[EI_MAG1] == ELFMAG1 && ehdr->e_ident[EI_MAG2] == ELFMAG2 && ehdr->e_ident[EI_MAG3] == ELFMAG3;
}

/*
  is_elf_machine_valid returns 1 if the machine specified by the header "ehdr"
  is valid and 0 if it isn't.
*/
int is_elf_machine_valid(Elf64_Ehdr* ehdr) {
  return ehdr->e_machine == 0x3e;
}

/*
  elf_segments_alloc allocates the ELF segments from the file specified by
  "file" and the Elf header specified by "ehdr" which have the flags "flags".
*/
struct list* elf_segments_alloc(struct file_info* file, const Elf64_Ehdr* ehdr, struct list** segments, int flags) {
  for (size_t i = 0; i < ehdr->e_phnum; ++i) {
    Elf64_Phdr* phdr = (Elf64_Phdr*)((uint64_t)file->buf.addr + ehdr->e_phoff + i * ehdr->e_phentsize);

    if (phdr->p_flags & flags) {
      struct segment_info* segment = malloc(sizeof(struct segment_info));

      segment->addr = phdr->p_vaddr;
      segment->buf.addr = (void*)((uint64_t)file->buf.addr + phdr->p_offset);
      segment->buf.size = phdr->p_filesz;
      segment->file = file;

      list_insert(segments, segment);
    }
  }

  return *segments;
}

/*
  elf_segments_free frees the elf segments "segments".
*/
void elf_segments_free(struct list** segments) {
  while ((*segments)->next) {
    free((*segments)->data);
    *segments = list_delete(segments);
  }
}
