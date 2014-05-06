#ifndef __SYM_H__
#define __SYM_H__

#include <stdint.h>

#include "mpu.h"

typedef struct _t_sym {
  char *name;
  uint16_t addr;
} t_sym;

typedef t_table t_sym_table;

t_sym *sym_get(t_sym_table *t, char *name);
int sym_get_idx(t_sym_table *t, char *name);
int sym_add(t_sym_table *t, char *name);
void sym_init(t_sym_table *t);
void sym_print(t_sym_table *t);

#endif//_SYM_H__
