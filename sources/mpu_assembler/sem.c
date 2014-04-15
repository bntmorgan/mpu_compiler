#include "sem.h"

void sem_mask(int size, t_reg op1, t_reg op2, t_reg op3, t_reg opt4) { }

void sem_equ(int size, t_reg op1, t_reg op2, t_reg op3, t_reg opt4) { }

void sem_inf(int size, t_reg op1, t_reg op2, t_reg op3) { } 

void sem_int(int size, t_reg op1) { }

void sem_mload(int size, t_reg op1) { }

void sem_load(int size, t_reg op1, int op2) { }

void sem_jmp(int size, t_reg op1) { } 

t_reg sem_reg(int reg, int sel) {
  t_reg r = {
    reg,
    sel
  };
  return r;
} 
