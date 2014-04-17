#include "mpu.h"
#include <stdlib.h>
#include <string.h>

#define MPU_SIZE_BASE 0x10

char *mnemonics[16] = {
  "",
  "mask",
  "equ",
  "inf",
  "int",
  "mload",
  "load",
  "jmp",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  ""
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
  2,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/**
 * Adds to a dyn table
 * idx : current idx,
 * size : size of one element,
 * nmemb : total size in number of elements
 */
int mpu_table_inc (uint8_t **t, size_t *idx, size_t size, size_t *nmemb) {
  if (*nmemb == 0) {
    *nmemb = MPU_SIZE_BASE;
    // printf("Allocating %lu bytes\n", size * *nmemb);
    *t = malloc(size * *nmemb);
    if (*t == NULL) {
      perror("Error allocating memory");
      return 1;
    }
  } else if (*idx >= *nmemb) {
    *nmemb *= 2;
    // printf("Reallocating %lu bytes\n", size * *nmemb);
    *t = realloc(*t, size * *nmemb);
    if (*t == NULL) {
      perror("Error allocating memory");
      return 1;
    }
  }
  return 0;
}

int mpu_disassemble (FILE *in, t_inst **inst, int **idx_to_addr, int
    **addr_to_idx, size_t *inst_len) {
  uint32_t r, tr, off = 0;
  uint8_t is;
  t_inst i;
  // Dyn tables
  size_t l_inst_nmemb = 0;
  size_t l_inst_idx = 0;
  t_inst *l_inst = NULL;
  size_t l_idx_nmemb = 0;
  size_t l_idx_idx = 0;
  int *l_idx = NULL;
  size_t l_ridx_nmemb = 0;
  size_t l_ridx_idx = 0;
  int *l_ridx = NULL;

  // Allocate for the first time
  mpu_table_inc((uint8_t **)&l_inst, &l_inst_idx, sizeof(t_inst),
      &l_inst_nmemb);
  mpu_table_inc((uint8_t **)&l_idx, &l_idx_idx, sizeof(t_inst), &l_idx_nmemb);
  mpu_table_inc((uint8_t **)&l_ridx, &l_ridx_idx, sizeof(t_inst),
      &l_ridx_nmemb);

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
    tr += is;
    // Copying the instruction
    l_inst[l_inst_idx] = i;
    // Idx to addr
    l_idx[l_idx_idx] = off;
    // Reverse index
    l_ridx[off] = l_inst_idx;
    // Increment idx
    l_idx_idx++;
    l_inst_idx++;
    l_ridx_idx += off + 1;
    mpu_table_inc((uint8_t **)&l_inst, &l_inst_idx, sizeof(t_inst), &l_inst_nmemb);
    mpu_table_inc((uint8_t **)&l_idx, &l_idx_idx, sizeof(t_inst), &l_idx_nmemb);
    mpu_table_inc((uint8_t **)&l_ridx, &l_ridx_idx, sizeof(t_inst), &l_ridx_nmemb);
    // Incrementing offset
    off += tr;
  }

  *inst = l_inst;
  *idx_to_addr = l_idx;
  *inst_len = l_inst_idx;
  *addr_to_idx = l_ridx;
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
      OP_INF || i->opcode.op == OP_LOAD){
    mpu_regfprintf(&i->op0, out, 0);
  } else {
    mpu_regfprintf(&i->op0, out, 1);
  }
  if (i->opcode.op == OP_LOAD) {
    fprintf(out, "0x%x", i->imm);
  } else {
    if (i->opcode.op == OP_MASK || i->opcode.op == OP_EQU || i->opcode.op ==
        OP_INF) {
      mpu_regfprintf(&i->op1, out, 0);
      mpu_regfprintf(&i->op2, out, 0);
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
    s += i->opcode.size + 1;
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
      OP_INF || i->opcode.op == OP_INT || i->opcode.op == OP_MLOAD ||
      i->opcode.op == OP_LOAD || i->opcode.op == OP_JMP, "Bad i->opcode.op \
      code");
}
