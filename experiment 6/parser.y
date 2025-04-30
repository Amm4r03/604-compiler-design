%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
%}

%token ID NUM
%left '+' 
%left '*'

%%
stmt: ID '=' expr ';' { printf("Valid assignment statement.\n"); }
    ;

expr: expr '+' term
    | term
    ;

term: term '*' factor
    | factor
    ;

factor: ID
      | NUM
      | '(' expr ')'
      ;

%%
int main() {
    printf("Enter a statement:\n");
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}
