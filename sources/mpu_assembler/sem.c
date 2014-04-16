#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "sem.h"
#include "parser.h"
#include "opt.h"

int csize;

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

t_reg sem_reg(int reg, int sel) {
  t_reg r = {
    .reg = reg,
    .sel = sel
  };
  assert_reg_size_sel(csize, reg, sel);
  return r;
}

/**
 * Compilation
 */
void compile(t_inst *i) {
  if (fwrite(i, isize(i), 1, file_out) <= 0) {
    log_error("Failed to compile");
    error();
  }
}

/**
 * Error handling
 */
void error(void) {
  exit(1);
}
