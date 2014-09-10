#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "sem.h"
#include "sym.h"
#include "parser.h"
#include "opt.h"

int csize;
t_sym_table t;
t_table ta;
uint16_t caddr;

typedef struct _t_temp_addr {
  char *name; // Name of the corresponding symbol
  int off; // Offset in the file
  int size; // Size in bytes
} t_temp_addr;

void sem_init() {
  caddr = 0;
  mpu_table_init(&ta, sizeof(t_temp_addr));
}

void sem_temp_addr_add(char *name, int off, int size) {
  t_temp_addr tmp = {
    name,
    off,
    size
  };
  table_get(&ta, t_temp_addr) = tmp;
  ta.idx++;
  mpu_table_inc(&ta);
}

/**
 * Parser Actions
 */

void sem_mask(t_inst *i) {
  compile(i);
}

void sem_equ(t_inst *i) {
  compile(i);
}

void sem_inf(t_inst *i) {
  compile(i);
}

void sem_add(t_inst *i) {
  compile(i);
}

void sem_hamm(t_inst *i) {
  compile(i);
}

void sem_int(t_inst *i) {
  compile(i);
}

void sem_mload(t_inst *i) {
  compile(i);
}

void sem_load(t_inst *i) {
  compile(i);
}

void sem_jmp(t_inst *i) {
  compile(i);
}

void sem_label(char *l) {
  if (sym_add(&t, l)) {
    log_error("Symbol %s already declared\n", l); 
  }
  table_get_idx(&t, t_sym, t.idx - 1).addr = caddr;
}

t_reg sem_reg(int reg, int sel) {
  t_reg r = {
    .reg = reg,
    .sel = sel
  };
  mpu_assert_reg_size_sel(csize, reg, sel);
  return r;
}

/**
 * Compilation
 */
void compile(t_inst *i) {
  size_t rs =  mpu_isize(i), s;
  if (i->opcode.op == OP_LOAD && i->is_sym == 1) {
    s = isizes[i->opcode.op];
  } else {
    s = rs;
  }
  if (fwrite(i, s, 1, file_out) <= 0) {
    log_error("Failed to compile");
    error();
  }
  caddr += rs;
  if (i->is_sym == 1) {
    int space = 0xcacacaca; 
    int imm_size = mpu_pow(2, i->opcode.size);
    fwrite(&space, sizeof(char), imm_size, file_out);
    sem_temp_addr_add(i->sym, caddr - imm_size, imm_size);
  }
}

int pad(void) {
  uint32_t zero = 0; 
  int padsize = caddr % 4;
  printf("PADDING %x\n", padsize);
  fwrite(&zero, sizeof(char), padsize, file_out);
  return padsize;
}

void sem_second_pass() {
  int i;
  sym_print(&t);
  printf("Second pass :\n");
  for (i = 0; i < ta.idx; i++) {
    t_temp_addr tmpa = table_get_idx(&ta, t_temp_addr, i);
    t_sym *sym = sym_get(&t, tmpa.name);
    printf("name %s, off %08x, size %02x\n", tmpa.name, tmpa.off, tmpa.size);
    if (sym == NULL) {
      log_error("Undeclared symbol %s\n", tmpa.name);
    }
    fseek(file_out, tmpa.off, SEEK_SET);
    fwrite(&sym->addr, tmpa.size, 1, file_out); 
  }
}

/**
 * Error handling
 */
void error(void) {
  exit(1);
}
