#include <stdio.h>
#include "mpu.h"
#include "opt.h"

int main(int argc, char *argv[]) {
  do_options(argc, argv);
  mpu_disassemble(infile, NULL, NULL);
  close_files();
  return 0;
}
