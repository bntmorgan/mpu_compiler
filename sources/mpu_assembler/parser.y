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
  char *string;
};

// Definition des types des tokens
%token <integer> tINTEGER tERROR tMASK tEQU tINF tADD tINT tMLOAD tLOAD tJMP 
%token <integer> tREG tREGSEP tB tW tD tQ tC
%token <string> tULABEL tDLABEL

%type <reg> reg
%type <integer> size

// Axiome
%start instructions

%%

instructions  : instruction instructions;
              | ;

instruction : tMASK  size reg tC reg tC reg tC reg {
  t_inst i = {
    .opcode = {
      .op = OP_MASK,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .op1 = $5,
    .op2 = $7,
    .op3 = $9
  };
  sem_mask(&i);
}
            | tEQU   size reg tC reg tC reg tC reg {
  t_inst i = {
    .opcode = {
      .op = OP_EQU,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .op1 = $5,
    .op2 = $7,
    .op3 = $9
  };
  sem_equ(&i);
}
            | tINF   size reg tC reg tC reg {
  t_inst i = {
    .opcode = {
      .op = OP_INF,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .op1 = $5,
    .op2 = $7,
    .op3.raw = 0
  };
  sem_inf(&i);
}
            | tADD   size reg tC reg tC reg {
  t_inst i = {
    .opcode = {
      .op = OP_ADD,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .op1 = $5,
    .op2 = $7,
    .op3.raw = 0
  };
  sem_inf(&i);
}
            | tINT   size reg {
  t_inst i = {
    .opcode = {
      .op = OP_INT,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .op1.raw = 0,
    .op2.raw = 0,
    .op3.raw = 0
  };
  sem_int(&i);
}
            | tMLOAD size reg {
  t_inst i = {
    .opcode = {
      .op = OP_MLOAD,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .op1.raw = 0,
    .op2.raw = 0,
    .op3.raw = 0
  };
  sem_mload(&i);
}
            | tLOAD  size reg tC tINTEGER {
  t_inst i = {
    .opcode = {
      .op = OP_LOAD,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .imm = $5,
    .is_sym = 0
  };
  sem_load(&i);
}
            | tLOAD  size reg tC tULABEL {
  t_inst i = {
    .opcode = {
      .op = OP_LOAD,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .sym = $5,
    .is_sym = 1
  };
  sem_load(&i);
}
            | tJMP   size reg {
  t_inst i = {
    .opcode = {
      .op = OP_JMP,
      .sop = 0,
      .size = $2
    },
    .op0 = $3,
    .op1.raw = 0,
    .op2.raw = 0,
    .op3.raw = 0
  };
  sem_jmp(&i);
}
            | tDLABEL {
  sem_label($1);
}

size  : tB {$$ = BYTE; csize = BYTE;}
      | tW {$$ = WORD; csize = WORD;}
      | tD {$$ = DWORD; csize = DWORD;}
      | tQ {$$ = QWORD; csize = QWORD;}

reg : tREG tINTEGER {
  $$ = sem_reg($2, 0);
}
    | tREG tINTEGER tREGSEP tINTEGER {
  $$ = sem_reg($2, $4);
}
%%
