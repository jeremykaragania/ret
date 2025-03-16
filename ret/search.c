#include <search.h>
#include <stdio.h>

static int invalid_mnemonics[INVALID_MNEMONICS_SIZE] = {UD_Ileave, UD_Iinvalid};

/*
  print_search prints the ROP gadgets in the segments specified by the search
  information "search" and the segments "segments".
*/
void print_search(struct search_info* search, struct list* segments) {
  size_t i = 0;
  struct list* curr = segments;
  const char* format = "%016lx: ";
  struct instruction_info* insns;
  size_t invalid_count = 0;
  int do_print = 1;

  if (search->gadget_length == 0) {
    return;
  }

  insns = malloc(search->gadget_length * sizeof(struct instruction_info));

  while(curr) {
    struct segment_info* segment = (struct segment_info*)curr->data;
    ud_t u;

    ud_init(&u);
    ud_set_input_buffer(&u, segment->buf.addr, segment->buf.size);
    ud_set_mode(&u, 64);
    ud_set_syntax(&u, UD_SYN_ATT);
    ud_set_asm_buffer(&u, insns[i].buf, 128);

    if (!search->has_base) {
      search->base = segment->addr;
    }

    while(ud_disassemble(&u)) {
      insns[i].mnemonic = ud_insn_mnemonic(&u);
      insns[i].off = ud_insn_off(&u);

      /*
        If the current instruction is invalid then we don't print anything in
        the instruction cache until it leaves the cache.
      */
      for (size_t j = 0; j < INVALID_MNEMONICS_SIZE; ++j) {
        if (insns[i].mnemonic == invalid_mnemonics[j]) {
          do_print = 0;
          invalid_count = 0;
        }
      }

      /*
        If there are no invalid instructions in the cache and the current
        instruction is a return instruction, then print the previous
        instructions.
      */
      if (do_print && insns[i].mnemonic == UD_Iret) {
        size_t j = 0;
        size_t k = (i + 1 + j) % search->gadget_length;

        printf(format, search->base + insns[k].off);

        while (j < search->gadget_length - 1) {
          printf("%s; ", insns[k].buf);

          ++j;
          k = (i + 1 + j) % search->gadget_length;
        }

        printf("%s\n", insns[i].buf);

        struct instruction_info* insn = &insns[(k - 1) % search->gadget_length];
        if (strcmp(insn->buf, "pop %r15") == 0) {
          printf(format, search->base + insn->off + 1);
          printf("pop %%rdi; ret\n");
        }
      }

      /*
        If there is an invalid instruction in the cache, then we increment the
        counter until it leaves.
      */
      if (!do_print) {
        ++invalid_count;
      }

      /*
        If an invalid instruction has left the cache then we can print again.
      */
      if (invalid_count == search->gadget_length) {
        do_print = 1;
      }

      i = (i + 1) % search->gadget_length;
      ud_set_asm_buffer(&u, insns[i].buf, 128);
    }

    curr = curr->next;
  }

  free(insns);
}
