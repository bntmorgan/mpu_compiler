#ifndef __SEM_H__
#define __SEM_H__

typedef struct _t_reg {
  int reg;
  int sel;
} t_reg;

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

void sem_mask(int size, t_reg *op1, t_reg *op2, t_reg *op3, t_reg *op4);
void sem_equ(int size, t_reg *op1, t_reg *op2, t_reg *op3, t_reg *op4);
void sem_inf(int size, t_reg *op1, t_reg *op2, t_reg *op3); 
void sem_int(int size, t_reg *op1);
void sem_mload(int size, t_reg *op1);
void sem_load(int size, t_reg *op1, int op2);
void sem_jmp(int size, t_reg *op1); 
t_reg sem_reg(int reg, int sel); 

void assert_reg_size_sel(int size, t_reg *op);
void assert_size_int(int size, int op);
void assert_size(int size);
void sem_error(void);

void compile(const char *format, ...);

#endif//__SEM_H__
