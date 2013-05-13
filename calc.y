%{
#include <stdio.h>
#include <stdlib.h>
#include "create.h"

%}
%union {
  struct expression_tree* val;
  struct expression_tree* expression;
}
%token <val> NUMBER
%token ADD SUB MUL DIV CR EQ LP RP POW REM
%type <expression> expression term primary_expression fact
%%
line_list
  : line
  | line_list line
  ;
line
  : expression CR
  {
    printf("\t%.10g\n", eval($1));
  }
  ;
expression
  : term
  | expression ADD term
  {
    $$ = create_expression(ADD_EXPRESSION, $1, $3);
  }
  | expression SUB term
  {
    $$ = create_expression(SUB_EXPRESSION, $1, $3);
  }
  ;
term
  : fact
  | term MUL fact
  {
	$$ = create_expression(MUL_EXPRESSION, $1, $3);
  }
  | term DIV fact
  {
	$$ = create_expression(DIV_EXPRESSION, $1, $3);	
  }
  ;
fact
  : LP expression RP
  {
	$$ = $2;
  }
  | primary_expression
  ;
primary_expression
  : NUMBER
  {
	$$ = $1;
  }
  ;
%%
int
yyerror(char const *str)
{
    extern char *yytext;
    fprintf(stderr, "parser error near %s\n", yytext);
    return 0;
}

int main(void)
{
    extern int yyparse(void);
    extern FILE *yyin;

    yyin = stdin;
    if (yyparse()) {
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
}
