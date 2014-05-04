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
  OP_ADD = 4,
  OP_INT = 12,
  OP_MLOAD = 13,
  OP_LOAD = 14,
  OP_JMP = 15
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

int mpu_disassemble (FILE *in, t_inst **inst, int **idx_to_addr, int
    **addr_to_idx, size_t *inst_len);
void mpu_print_program(t_inst *inst, int *idx_to_addr, size_t inst_len);
uint8_t mpu_isize (t_inst *i);

void mpu_assert_reg_size_sel(uint32_t size, uint32_t  reg, uint32_t sel);
void mpu_assert_size_int(t_inst *i, int op);
void mpu_assert_opcode(t_inst *i);
void mpu_ifprintf(t_inst *i, FILE *out);
void mpu_regfprintf(t_reg *r, FILE *out, int last);

#endif//__MPU_H__
