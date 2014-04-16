#ifndef __MPU_H__
#define __MPU_H__

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) {fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n" \
    , __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); error();}
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__);}

extern void error(void);

enum OP_SIZE {
  BYTE,
  WORD,
  DWORD,
  QWORD
};

enum OP_CODE {
  OP_MASK = 1,
  OP_EQU = 2,
  OP_INF = 3,
  OP_INT = 4,
  OP_MLOAD = 5,
  OP_LOAD = 6,
  OP_JMP = 7
};

enum ISIZE {
  SIZE_MASK = 5,
  SIZE_EQU = 5,
  SIZE_INF = 4,
  SIZE_INT = 2,
  SIZE_MLOAD = 2,
  SIZE_LOAD_BASE = 2,
  SIZE_JMP = 2
};

typedef struct _t_reg {
  union {
    struct {
      uint8_t sel:3;
      uint8_t reg:5;
    };
    uint8_t raw;
  };
} __attribute__((packed)) t_reg;

typedef struct _t_opcode {
  union {
    struct {
      uint8_t size:2;
      uint8_t sop:2;
      uint8_t op:4;
    };
    uint8_t raw;
  };
} __attribute__((packed)) t_opcode;

typedef struct _t_inst {
  t_opcode opcode;
  t_reg op0;
  union {
    struct {
      t_reg op1;
      t_reg op2;
      t_reg op3;
    };
    uint32_t imm;
  };
} __attribute__((packed)) t_inst;

int disassemble (FILE *in, t_inst *instructions, ssize_t *size);
uint8_t isize (t_inst *i);

void assert_reg_size_sel(uint32_t size, uint32_t  reg, uint32_t sel);
void assert_size_int(t_inst *i, int op);
void assert_opcode(t_inst *i);

#endif//__MPU_H__
