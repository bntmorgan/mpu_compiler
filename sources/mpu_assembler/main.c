#include <stdio.h>
#include "sem.h"
#include "parser.h"

extern int line;

void yyerror(const char *s) {
  fprintf(stderr, "Vous ne maîtrisez pas les concepts : %s at line %d\n", s, line);
}

int main(int argc, char *argv[]) {
  yyparse();
  return 0;
}
