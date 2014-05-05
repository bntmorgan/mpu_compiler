#include <stdio.h>
#include "mpu.h"
#include "opt.h"

int main(int argc, char *argv[]) {
  t_table inst;
  t_table idx_to_addr;
  t_table addr_to_idx;

  mpu_table_init(&inst, sizeof(t_inst));
  mpu_table_init(&idx_to_addr, sizeof(int));
  mpu_table_init(&addr_to_idx, sizeof(int));

  do_options(argc, argv);
  mpu_disassemble(infile, &inst, &idx_to_addr, &addr_to_idx);
  mpu_print_program(inst.t, idx_to_addr.t, inst.idx);
  close_files();
  return 0;
}
