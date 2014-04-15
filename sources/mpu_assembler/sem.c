#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sem.h"
#include "parser.h"

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) {fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n" \
    , __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); sem_error();}
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__);}

/**
 * Parser Actions
 */

void sem_mask(int size, t_reg *op1, t_reg *op2, t_reg *op3, t_reg *op4) {
  assert_size(size);
  assert_reg_size_sel(size, op1);
  assert_reg_size_sel(size, op2);
  assert_reg_size_sel(size, op3);
  assert_reg_size_sel(size, op4);
}

void sem_equ(int size, t_reg *op1, t_reg *op2, t_reg *op3, t_reg *op4) {
  assert_size(size);
  assert_reg_size_sel(size, op1);
  assert_reg_size_sel(size, op2);
  assert_reg_size_sel(size, op3);
  assert_reg_size_sel(size, op4);
}

void sem_inf(int size, t_reg *op1, t_reg *op2, t_reg *op3) {
  assert_size(size);
  assert_reg_size_sel(size, op1);
  assert_reg_size_sel(size, op2);
  assert_reg_size_sel(size, op3);
}

void sem_int(int size, t_reg *op1) {
  assert_size(size);
  assert_reg_size_sel(size, op1);
}

void sem_mload(int size, t_reg *op1) {
  assert_size(size);
  assert_reg_size_sel(size, op1);
}

void sem_load(int size, t_reg *op1, int op2) {
  assert_size(size);
  assert_reg_size_sel(size, op1);
  assert_size_int(size, op2);
}

void sem_jmp(int size, t_reg *op1) {
  assert_size(size);
  assert_reg_size_sel(size, op1);
}

t_reg sem_reg(int reg, int sel) {
  t_reg r = {
    reg,
    sel
  };
  return r;
}

/**
 * Semantics
 */

void assert_reg_size_sel(int size, t_reg *op) {
  assertf(op->reg >= 0 && op->reg <= 31, "GPR number out of bounds");
  switch (size) {
    case BYTE:
      assertf(op->sel >= 0 && op->sel <= 7, "Selector out of bounds");
      break;
    case WORD:
      assertf(op->sel >= 0 && op->sel <= 3, "Selector out of bounds");
      break;
    case DWORD:
      assertf(op->sel >= 0 && op->sel <= 1, "Selector out of bounds");
      break;
    case QWORD:
      assertf(op->sel == 0, "Selector out of bounds");
      break;
    default:
      ;
      // Error
  }
}

void assert_size_int(int size, int op) {
  unsigned int o = (unsigned int)op;
  switch (size) {
    case BYTE:
      assertf(o >= 0 && o <= 0xff, "Integer out of bounds");
      break;
    case WORD:
      assertf(o >= 0 && o <= 0xffff, "Integer out of bounds");
      break;
    case DWORD:
      assertf(o >= 0 && o <= 0xffffffff, "Integer out of bounds");
      break;
    case QWORD:
      assertf(o >= 0 && o <= 0xffffffffffffffff, "Integer out of bounds");
      break;
    default:
      ;
      // Error
  }
}

void assert_size(int size) {
  assertf(size == BYTE || size == WORD || size == DWORD || size == QWORD, "Op \
      size out of bouds");
}

/**
 * Error handling
 */
void sem_error(void) {
  exit(1);
}
