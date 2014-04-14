#ifndef __SEM_H__
#define __SEM_H__

void sem_mask(int op1, int op2, int op3, int opt4);
void sem_equ(int op1, int op2, int op3, int opt4);
void sem_inf(int op1, int op2, int op3);
void sem_int(int op1);
void sem_mload(int op1);
void sem_load(int op1, int op2);
void sem_jmp(int op1);

#endif//__SEM_H__
