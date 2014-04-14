%{
  #include "parser.h" 
  extern int line;
  #define YY_NO_INPUT
%}

WHITESPACE [ \t]

%x COMMENT

%option nounput
%%

"/*" {BEGIN COMMENT;}
<COMMENT>\n {line++;}
<COMMENT>[.]* {}
<COMMENT>"*/" {BEGIN INITIAL;printf("\n");}

{WHITESPACE} {}
\n {line++;}
. {return tERROR;}

%%

int line = 1;
