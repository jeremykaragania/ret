#include <search.h>
#include <stdio.h>

/*
  print_search prints the ROP gadgets in the segments specified by "segments".
*/
void print_search(struct list* segments) {
  /*
    Since we can only disassemble one instruction at a time, we have to keep
    two buffers: one for the current and previous instruction.
  */
  int i = 0;
  struct list* curr = segments;
  const char* format_0 = "%016lx: %s\n";
  const char* format_1 = "\033[0;94m%016lx\033[0m: %s\n";
  char asm_buf_0[128];
  char asm_buf_1[128];
  char* asm_bufs[2] = {
    asm_buf_0,
    asm_buf_1
  };

  /*
    If stdout doesn't refer to a terminal, then we don't print ANSI escape
    sequences.
  */
  if (isatty(STDOUT_FILENO)) {
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
      ud_set_asm_buffer(&u, asm_bufs[i], 128);

      /*
        If the current instruction is a return instruction, then print the
        previous instruction.
      */
      if (ud_insn_mnemonic(&u) == UD_Iret) {
        printf(format_0, segment->addr + ud_insn_off(&u), asm_bufs[i]);
      }

      i = !i;
    }

    curr = curr->next;
  }
}
