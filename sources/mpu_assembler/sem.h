#ifndef __SEM_H__
#define __SEM_H__

#include "mpu.h"

extern int csize;

void sem_init();
void sem_mask(t_inst *i);
void sem_equ(t_inst *i);
void sem_inf(t_inst *i);
void sem_add(t_inst *i);
void sem_hamm(t_inst *i);
void sem_int(t_inst *i);
void sem_mload(t_inst *i);
void sem_load(t_inst *i);
void sem_jmp(t_inst *i);
t_reg sem_reg(int reg, int sel); 
void sem_label(char *l);

void sem_error(void);

void compile(t_inst *i);
int pad(void);
void sem_second_pass();

#endif//__SEM_H__
