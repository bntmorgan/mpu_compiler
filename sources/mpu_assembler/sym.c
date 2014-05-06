#include <string.h>

#include "sym.h"
#include "mpu.h"

void sym_init(t_sym_table *t) {
  mpu_table_init(t, sizeof(t_sym));
}

t_sym *sym_get(t_sym_table *t, char *name) {
  int i;
  for (i = 0; i < t->idx; i++) {
    if (strcmp(table_get_idx(t, t_sym, i).name, name) == 0) {
      return &table_get_idx(t, t_sym, i);
    }
  }
  return NULL;
}

int sym_get_idx(t_sym_table *t, char *name) {
  int i;
  for (i = 0; i < t->idx; i++) {
    if (strcmp(table_get_idx(t, t_sym, i).name, name) == 0) {
      return i;
    }
  }
  return -1;
}

int sym_add(t_sym_table *t, char *name) {
  t_sym s = {
    .name = name,
    .addr = 0
  };
  if (sym_get(t, name) != NULL) {
    return -1;
  }
  table_get(t, t_sym) = s;
  t->idx++;
  mpu_table_inc(t);
  return 0;
}

void sym_print(t_sym_table *t) {
  int i;
  printf("Symbol table :\n");
  for (i = 0; i < t->idx ; i++) {
    printf("%s 0x%04x\n", table_get_idx(t, t_sym, i).name, table_get_idx(t,
          t_sym, i).addr);
  }
}
