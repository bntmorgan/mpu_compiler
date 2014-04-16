#include "mpu.h"
#include <stdlib.h>
#include <string.h>

int disassemble (FILE *in, t_inst *instructions, ssize_t *size) {
  // int offset = 0;
  int r;
  uint8_t is;
  t_inst i;

  // Lets go baby
  while (1) {
    r = fread(&i, sizeof(t_opcode), 1, in);
    if (r <= 0) {
      // This is the end, the correct end YOLORD
      return 0;
    }
    assert_opcode(&i);
    is = isize(&i);
    r = fread(((uint8_t*)&i) + sizeof(t_opcode), is - sizeof(t_opcode), 1, in); 
    if (r <= 0) {
      fprintf(stderr, "[ERROR] Failed to read input file :(\n");
      return 1;
    }
  }

  return 0;
}

uint8_t isize (t_inst *i) {
  switch (i->opcode.op) {
  case OP_MASK:
    return SIZE_MASK;
  case OP_EQU:
    return SIZE_EQU;
  case OP_INF:
    return SIZE_INF;
  case OP_INT:
    return SIZE_INT;
  case OP_MLOAD:
    return SIZE_MLOAD;
  case OP_LOAD:
    return SIZE_LOAD_BASE + i->opcode.size + 1;
  case OP_JMP:
    return SIZE_JMP;
  default:
    ;
    // Error
  }
  return 0;
}

/**
 * Semantics
 */

void assert_reg_size_sel(uint32_t size, uint32_t  reg, uint32_t sel) {
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

void assert_size_int(t_inst *i, int op) {
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

void assert_opcode(t_inst *i) {
  assertf(i->opcode.op == OP_MASK || i->opcode.op == OP_EQU || i->opcode.op ==
      OP_INF || i->opcode.op == OP_INT || i->opcode.op == OP_MLOAD ||
      i->opcode.op == OP_LOAD || i->opcode.op == OP_JMP, "Bad i->opcode.op \
      code");
}
