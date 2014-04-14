%{
  #include "parser.h"

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
%token <entier> tINTEGER
%token <chaine> tWORD
%token <entier> tERROR

// Axiome
%start instructions

%%

instructions: ;

%%
