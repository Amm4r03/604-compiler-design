%{
#include "parser.tab.h"
%}

%%
[a-zA-Z_][a-zA-Z0-9_]*   { yylval.id = strdup(yytext); return ID; }
"="                     { return '='; }
"+"                     { return '+'; }
";"                     { return ';'; }
[ \t\n]+                { /* skip whitespace */ }
.                       { return yytext[0]; }
%%
