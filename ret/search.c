#include <search.h>

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
  char asm_buf_0[128];
  char asm_buf_1[128];
  char* asm_bufs[2] = {
    asm_buf_0,
    asm_buf_1
  };


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
        printf("%016lx: %s\n", segment->addr + ud_insn_off(&u), asm_bufs[i]);
      }

      i = !i;
    }

    curr = curr->next;
  }
}
