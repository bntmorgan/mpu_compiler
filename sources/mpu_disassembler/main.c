#include <stdio.h>
#include "mpu.h"
#include "opt.h"

int main(int argc, char *argv[]) {
  t_inst *inst;
  int *idx_to_addr;
  int *addr_to_idx;
  size_t inst_len;
  do_options(argc, argv);
  mpu_disassemble(infile, &inst, &idx_to_addr, &addr_to_idx, &inst_len);
  close_files();
  return 0;
}
