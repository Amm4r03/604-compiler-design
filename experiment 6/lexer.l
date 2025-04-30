%{
#include "parser.tab.h"
%}

%%
[ \t\n]+                 ;  // skip whitespace
[a-zA-Z_][a-zA-Z0-9_]*   { return ID; }
[0-9]+                  { return NUM; }
"="                     return '=';
";"                     return ';';
"+"                     return '+';
"*"                     return '*';
"("                     return '(';
")"                     return ')';

.                       { printf("Unknown character: %s\n", yytext); }
%%
