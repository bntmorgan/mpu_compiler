%{
  #include "sem.h"
  #include "parser.h"

  extern void yyerror(const char *s);
  extern int yylex();
%}

%error-verbose

// Declaration des types utilisés
%union {
  int integer;
  t_reg reg;
};

// Definition des types des tokens
%token <integer> tINTEGER tERROR tMASK tEQU tINF tINT tMLOAD tLOAD tJMP tREG 
%token <integer> tREGSEP tB tW tD tQ tC

%type <reg> reg
%type <integer> size

// Axiome
%start instructions

%%

instructions  : instruction instructions;
              | ;

instruction : tMASK  size reg tC reg tC reg tC reg {
  sem_mask($2, &$3, &$5, &$7, &$9);
}
            | tEQU   size reg tC reg tC reg tC reg {
  sem_equ($2, &$3, &$5, &$7, &$9);
}
            | tINF   size reg tC reg tC reg {
  sem_inf($2, &$3, &$5, &$7);
}
            | tINT   size reg {
  sem_int($2, &$3);
}
            | tMLOAD size reg {
  sem_mload($2, &$3);
}
            | tLOAD  size reg tC tINTEGER {
  sem_load($2, &$3, $5);
}
            | tJMP   size reg {
  sem_jmp($2, &$3);
}

size  : tB {$$ = BYTE;}
      | tW {$$ = WORD;}
      | tD {$$ = DWORD;}
      | tQ {$$ = QWORD;}

reg : tREG tINTEGER {
  $$ = sem_reg($2, 0);
}
    | tREG tINTEGER tREGSEP tINTEGER {
  $$ = sem_reg($2, $4);
}
%%
