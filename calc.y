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
  struct condition* condition;
}
%token <val> NUMBER
%token <name> IDENTIFIER
%token ADD SUB MUL DIV CR EQ LP RP POW REM COMMA IF COLON EQL N_EQL LS GR LS_E GR_E SIN COS TAN DEF
%type <expression> expression term primary_expression fact if_statement statement
%type <condition> conditions
%type <args> arguments params
%%
line_list
  : line
  | line_list line
  ;
line
  : statement CR
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
statement
  : expression
  | if_statement
  ;
function_definition
  : DEF IDENTIFIER LP params RP statement
  {
	define_function($2, $4, $6);
  }
  ;
assignment
  : IDENTIFIER EQ statement
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
  | params COMMA IDENTIFIER
  {
	$$ = chain_param($1, $3);
  }
  ;
arguments
  : expression
  {
	Arguments args = generate_arg_list();
	$$ = chain_arg(args, $1);
  }
  | arguments COMMA expression
  {
	$$ = chain_arg($1, $3);
  }
  ;
if_statement
  : conditions IF statement COLON statement
  {
	$$ = create_if_expression($1, $3, $5);
  }
  ;
conditions
  : expression EQL expression
  {
	$$ = create_condition(EQUAL, $1, $3);
  }
  | expression N_EQL expression
  {
	$$ = create_condition(NOT_EQUAL, $1, $3);
  }
  | expression LS expression
  {
	$$ = create_condition(LESS, $1, $3);
  }
  | expression GR expression
  {
	$$ = create_condition(GREATER, $1, $3);
  }
  | expression LS_E expression
  {
	$$ = create_condition(LESS_EQUAL, $1, $3);
  }
  | expression GR_E expression
  {
	$$ = create_condition(GREATER_EQUAL, $1, $3);
  }
  ;
expression
  : term
  | if_statement
  {
	$$ = $1;
  }
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
  | term POW fact
  {
	$$ = create_expression(POW_EXPRESSION, $1, $3);
  }
  | term REM fact
  {
	$$ = create_expression(REM_EXPRESSION, $1, $3);
  }
  | SIN LP expression RP
  {
  $$ = create_expression(SIN_EXPRESSION, $3, NULL);
  }
  | COS LP expression RP
  {
  $$ = create_expression(COS_EXPRESSION, $3, NULL);
  }
  | TAN LP expression RP
  {
	$$ = create_expression(TAN_EXPRESSION, $3, NULL);
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
