%{
  #include "parser.h" 
  extern int line;
  #define YY_NO_INPUT
%}

WHITESPACE [ \t]
WORD [a-zA-Z0-9_]+
HEXA_INTEGER 0x[0-9a-fA-F]+
DEC_INTEGER [0-9]+(e-?[09]+)?
INTEGER {HEXA_INTEGER}|{DEC_INTEGER}

%x COMMENT

%option nounput

%%

"/*" {BEGIN COMMENT;}
<COMMENT>\n {line++;}
<COMMENT>[.]* {}
<COMMENT>"*/" {BEGIN INITIAL;printf("\n");}

(MASK|mask) {return tMASK;}
(EQU|equ) {return tEQU;}
(INF|inf) {return tINF;}
(INT|int) {return tINT;}
(MLOAD|mload) {return tMLOAD;}
(LOAD|load) {return tLOAD;}
(JMP|jmp) {return tJMP;}

{INTEGER} {yylval.integer = atoi(yytext); return tINTEGER;}
{WORD} {yylval.string = strdup(yytext); return tWORD;}
{WHITESPACE} {}
\n {line++;}
. {return tERROR;}

%%

int line = 1;
