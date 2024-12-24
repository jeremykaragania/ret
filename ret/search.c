#include <search.h>
#include <stdio.h>

/*
  print_search prints the ROP gadgets in the segments specified by the search
  information "search" and the segments "segments".
*/
void print_search(struct search_info* search, struct list* segments) {
  /*
    Since we can only disassemble one instruction at a time, we have to keep
    two buffers: one for the current and previous instruction.
  */
  int i = 0;
  struct list* curr = segments;
  const char* format_0 = "%016lx: %s\n";
  const char* format_1 = "\033[0;94m%016lx\033[0m: %s\n";
  struct instruction_info insns[2];

  /*
    If stdout refers to a terminal, then we print ANSI escape sequences.
  */
  if (isatty(fileno(stdout))) {
    format_0 = format_1;
  }

  while(curr) {
    struct segment_info* segment = (struct segment_info*)curr->data;
    ud_t u;

    ud_init(&u);
    ud_set_input_buffer(&u, segment->buf.addr, segment->buf.size);
    ud_set_mode(&u, 64);
    ud_set_syntax(&u, UD_SYN_ATT);

    while(ud_disassemble(&u)) {
      ud_set_asm_buffer(&u, insns[i].buf, 128);

      insns[i].mnemonic = ud_insn_mnemonic(&u);
      insns[i].off = ud_insn_off(&u);

      /*
        If the current instruction is a return instruction, then print the
        previous instruction.
      */
      if (insns[i].mnemonic == UD_Iret) {
        printf(format_0, search->base + segment->addr + insns[!i].off, insns[i].buf);
      }

      i = !i;
    }

    curr = curr->next;
  }
}
