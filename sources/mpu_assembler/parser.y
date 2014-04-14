%{
  #include "parser.h"
  #include "sem.h"

  extern void yyerror(const char *s);
  extern int yylex();
%}

%error-verbose

// Declaration des types utilisés
%union {
  int integer;
  char *string;
};

// Definition des types des tokens
%token <integer> tINTEGER tERROR tMASK tEQU tINF tINT tMLOAD tLOAD tJMP
%token <string> tWORD

// Axiome
%start instructions

%%

instructions  : instruction instructions;
              | ;

instruction : tMASK  tINTEGER tINTEGER tINTEGER tINTEGER {
  sem_mask($2, $3, $4, $5);
}
            | tEQU   tINTEGER tINTEGER tINTEGER tINTEGER {
  sem_equ($2, $3, $4, $5);
}
            | tINF   tINTEGER tINTEGER tINTEGER          {
  sem_inf($2, $3, $4);
}
            | tINT   tINTEGER                            {
  sem_int($2);
}
            | tMLOAD tINTEGER                            {
  sem_mload($2);
}
            | tLOAD  tINTEGER tINTEGER                   {
  sem_load($2, $3);
}
            | tJMP   tINTEGER                            {
  sem_jmp($2);
}

%%
