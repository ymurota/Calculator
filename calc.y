%{
#include <stdio.h>
#include <stdlib.h>
#include "create.h"

%}
%union {
  char name[100];
  struct arguments_link* args;
  struct expression_tree* val;
  struct expression_tree* expression;
}
%token <val> NUMBER
%token <name> IDENTIFIER
%token ADD SUB MUL DIV CR EQ LP RP POW REM DEF COMMA
%type <expression> expression term primary_expression fact
%type <args> arguments params
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
  | function_definition CR
  {
	printf("function defined\n");
  }
  | assignment CR
  {
	printf("assigned\n");
  }
  ;
function_definition
  : DEF IDENTIFIER  LP params RP expression
  {
	define_function($2, $4, $6);
  }
  ;
assignment
  : IDENTIFIER EQ expression
  {
	define_variable($1, $3);
  }
  ;
params
  : IDENTIFIER
  {
	Arguments args = generate_arg_list();
	$$ = chain_param(args, $1);
  }
  | IDENTIFIER COMMA params
  {
	$$ = chain_param($3, $1);
  }
  ;
arguments
  : NUMBER
  {
	Arguments args = generate_arg_list();
	$$ = chain_arg(args, eval($1));
  }
  | NUMBER COMMA arguments
  {
	$$ = chain_arg($3, eval($1));
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
  | IDENTIFIER
  {
	$$ = create_variable_expression($1);
  }
  | IDENTIFIER LP arguments RP
  {
	$$ = create_function_expression($1, $3);
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

	init_env();
    yyin = stdin;
    if (yyparse()) {
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
}
