%{
  #include "sem.h" 
  #include "parser.h" 
  extern int line;
  #define YY_NO_INPUT
%}

WHITESPACE [ \t]
WORD [a-zA-Z0-9_]+
HEXA_INTEGER 0x[0-9a-fA-F]+
DEC_INTEGER [0-9]+
INTEGER {HEXA_INTEGER}|{DEC_INTEGER}

%x COMMENT

%option nounput

%%

"/*" {BEGIN COMMENT;}
<COMMENT>\n {line++;}
<COMMENT>[.]* {}
<COMMENT>"*/" {BEGIN INITIAL;printf("\n");}

"//".*\n {}
(MASK|mask) {return tMASK;}
(EQU|equ) {return tEQU;}
(INF|inf) {return tINF;}
(ADD|add) {return tADD;}
(INT|int) {return tINT;}
(MLOAD|mload) {return tMLOAD;}
(LOAD|load) {return tLOAD;}
(JMP|jmp) {return tJMP;}
[rR] {return tREG;}
_ {return tREGSEP;}
[bB] {return tB;}
[wW] {return tW;}
[dD] {return tD;}
[qQ] {return tQ;}
[,] {return tC;}

{INTEGER} {yylval.integer = strtol(yytext, NULL, 0); return tINTEGER;}
{WHITESPACE} {}
\n {line++;}
. {return tERROR;}

%%

int line = 1;
