#include <stdio.h>
#include "sem.h"
#include "sym.h"
#include "opt.h"
#include "parser.h"

extern int line;
extern t_sym_table t;

void yyerror(const char *s) {
  fprintf(stderr, "Vous ne maîtrisez pas les concepts : %s at line %d\n", s,
      line);
}

int main(int argc, char *argv[]) {
  do_options(argc, argv);
  sym_init(&t);
  sem_init();
  yyparse();
  sem_second_pass();
  close_files();
  return 0;
}
