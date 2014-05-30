#include <stdlib.h>
#include <string.h>

#include "mpu.h"

#define MPU_SIZE_BASE 0x10

char *mnemonics[16] = {
  "",
  "mask",
  "equ",
  "inf",
  "add",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "int",
  "mload",
  "load",
  "jmp"
};

char *sizes[4] = {
  "b",
  "w",
  "d",
  "q"
};

int isizes[16] = {
  0,
  5,
  5,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  3,
  2,
  2
};

int mpu_pow(int x, int y) {
  int i, res = x;
  if (y == 0) {
    res = 1;
  } else {
    for (i = 0; i < y - 1; i++) {
      res *= x;
    }
  }
  return res;
}

/**
 * Adds to a dyn table
 * idx : current idx,
 * size : size of one element,
 * nmemb : total size in number of elements
 */
int mpu_table_inc (t_table *t) {
  if (t->nmemb == 0) {
    t->nmemb = MPU_SIZE_BASE;
    // printf("Allocating %lu bytes\n", size * *nmemb);
    t->t = malloc(t->size * t->nmemb);
    if (t == NULL) {
      perror("Error allocating memory");
      return 1;
    }
  } else if (t->idx >= t->nmemb) {
    t->nmemb *= 2;
    // printf("Reallocating %lu bytes\n", size * *nmemb);
    t->t = realloc(t->t, t->size * t->nmemb);
    if (t->t == NULL) {
      perror("Error allocating memory");
      return 1;
    }
  }
  return 0;
}

void mpu_table_init(t_table *t, size_t size) {
  t->nmemb = 0;
  t->idx = 0;
  t->size = size;
  t->t = NULL;

  // Allocate for the first time
  mpu_table_inc(t);
}

int mpu_disassemble (FILE *in, t_table *inst, t_table *idx, t_table *ridx) {
  uint32_t r, tr, off = 0;
  uint8_t is;
  t_inst i;

  // Lets go baby
  while (1) {
    tr = 0;
    memset(&i, 0, sizeof(t_inst));
    r = fread(&i, sizeof(t_opcode), 1, in);
    if (r <= 0) {
      // This is the end, the correct end YOLORD
      break;
    }
    mpu_assert_opcode(&i);
    is = mpu_isize(&i);
    r = fread(((uint8_t*)&i) + sizeof(t_opcode), is - sizeof(t_opcode), 1, in); 
    if (r <= 0) {
      fprintf(stderr, "[ERROR] Failed to read input file :(\n");
      return 1;
    }
    // TODO test the operands ?
    tr += is;
    // Copying the instruction
    table_get(inst, t_inst) = i;
    // Idx to addr
    table_get(idx, int) = off;
    // Reverse index
    table_get(ridx, int) = table_get(idx, int);
    // Increment idx
    idx->idx++;
    inst->idx++;
    ridx->idx += off + 1;
    mpu_table_inc(inst);
    mpu_table_inc(idx);
    mpu_table_inc(ridx);
    // Incrementing offset
    off += tr;
  }

  return 0;
}

void mpu_print_program(t_inst *inst, int *idx_to_addr, size_t inst_len) {
  int i;
  for (i = 0; i < inst_len; i++) {
    fprintf(stdout, "0x%04x: ", idx_to_addr[i]);
    mpu_ifprintf(&inst[i], stdout);
  }
}

void mpu_ifprintf(t_inst *i, FILE *out) {
  fprintf(out, "%s%s ", mnemonics[i->opcode.op], sizes[i->opcode.size]);
  if (i->opcode.op == OP_MASK || i->opcode.op == OP_EQU || i->opcode.op ==
      OP_INF || i->opcode.op == OP_ADD || i->opcode.op == OP_LOAD){
    mpu_regfprintf(&i->op0, out, 0);
  } else {
    mpu_regfprintf(&i->op0, out, 1);
  }
  if (i->opcode.op == OP_LOAD) {
    fprintf(out, "$0x%x", i->imm);
  } else {
    if (i->opcode.op == OP_MASK || i->opcode.op == OP_EQU) {
      mpu_regfprintf(&i->op1, out, 0);
      mpu_regfprintf(&i->op2, out, 0);
    } else if (i->opcode.op == OP_INF || i->opcode.op == OP_ADD) {
      mpu_regfprintf(&i->op1, out, 0);
      mpu_regfprintf(&i->op2, out, 1);
    }
    if (i->opcode.op == OP_MASK || i->opcode.op == OP_EQU) {
      mpu_regfprintf(&i->op3, out, 1);
    }
  }
  fprintf(out, "\n");
}

void mpu_regfprintf(t_reg *r, FILE *out, int last) {
  if (!r->sel) {
    fprintf(out, "r%d", r->reg);
  } else {
    fprintf(out, "r%d_%d", r->reg, r->sel);
  }
  if (!last) {
    fprintf(out, ", ");
  }
}

uint8_t mpu_isize (t_inst *i) {
  uint8_t s = isizes[i->opcode.op];
  if (i->opcode.op == OP_LOAD) {
    s += (uint8_t)mpu_pow(2, i->opcode.size);
  }
  return s;
}

/**
 * Semantics
 */

void mpu_assert_reg_size_sel(uint32_t size, uint32_t  reg, uint32_t sel) {
  assertf(reg >= 0 && reg <= 31, "GPR number out of bounds");
  switch (size) {
    case BYTE:
      assertf(sel >= 0 && sel <= 7, "Selector out of bounds");
      break;
    case WORD:
      assertf(sel >= 0 && sel <= 3, "Selector out of bounds");
      break;
    case DWORD:
      assertf(sel >= 0 && sel <= 1, "Selector out of bounds");
      break;
    case QWORD:
      assertf(sel == 0, "Selector out of bounds");
      break;
    default:
      ;
      // Error
  }
}

void mpu_assert_size_int(t_inst *i, int op) {
  unsigned int o = (unsigned int)op;
  switch (i->opcode.size) {
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

void mpu_assert_opcode(t_inst *i) {
  assertf(i->opcode.op == OP_MASK || i->opcode.op == OP_EQU || i->opcode.op ==
      OP_INF || i->opcode.op == OP_ADD || i->opcode.op == OP_INT || i->opcode.op
      == OP_MLOAD || i->opcode.op == OP_LOAD || i->opcode.op == OP_JMP, 
      "Bad opcode 0x%02x", i->opcode.op);
}
