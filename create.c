#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "create.h"

/* global variables */
Symbol local_env;
Symbol global_env;
Stack stack;

/* environment functions */
Symbol init_table(Symbol table) {
  table = (Symbol)malloc(sizeof *table);
  table->next = NULL;
  return table;
}

void init_env() {
  global_env = init_table(global_env);
  local_env = init_table(local_env);
}

void push_env(Symbol env) {
  stack[pointer++] = env;
}

Symbol pop_env() {
  return stack[--pointer];
}

/* create functions */
Expression create_expression(ExpressionType type, Expression lft, Expression rght) {
  Expression exp = (Expression) malloc(sizeof *exp);
  exp->type = type;
  exp->lft = lft;
  exp->rght = rght;
  return exp;
}

Expression create_value_expression(double val) {
  Expression exp = (Expression) malloc(sizeof *exp);
  exp->type = VALUE;
  exp->val = val;
  return exp;
}

Expression create_variable_expression(char* name) {
  Expression var = (Expression) malloc(sizeof *var);
  var->type = VARIABLE;
  var->identifier = (char*) malloc(sizeof(char)*(strlen(name)+1));
  strcpy(var->identifier, name);
  return var;
}

Expression create_function_expression(char* name, Arguments args) {
  printf ("create_function:%s\n",args->name);
  Expression func = (Expression) malloc(sizeof *func);
  func->type = FUNCTION;
  func->identifier = (char*) malloc(sizeof(char)*(strlen(name)+1));
  strcpy(func->identifier, name);
  func->args = args;

  return func;
}

/* eval functions */
double eval(Expression tree) {
  if (tree->type != VALUE) {
	Symbol s;
	switch (tree->type) {
	case ADD_EXPRESSION:
	  return eval(tree->lft) + eval(tree->rght);
	case SUB_EXPRESSION:
	  return eval(tree->lft) - eval(tree->rght);
	case MUL_EXPRESSION:
	  return eval(tree->lft) * eval(tree->rght);
	case DIV_EXPRESSION:
	  return eval(tree->lft) / eval(tree->rght);
	case FUNCTION:
	  return function_call(tree->identifier, tree->args);
	case VARIABLE:
	  s = find_varivales(tree->identifier);
	  return s->exp->val;
	}
  } else {
	return tree->val;
  }
}

/* define functions */
void define_function(char* name, Arguments params, Expression exp) {
  global_env = putsym(global_env, FUNCTION, name, exp, params);
}

void define_variable(char* name, Expression exp) {
  global_env = putsym(global_env, VARIABLE, name, exp, NULL);
}

/* function call sequence */
double function_call(char* name, Arguments args) {
  double ret;
  Symbol func = find_function(name);
  save_local();
  set_arguments_to_local(func->params, args);

  ret = eval(func->exp);

  set_local();
  return ret;
}

void set_arguments_to_local(Arguments params, Arguments args) {
  Expression v;
  for(; params->next != NULL || args->next != NULL; params = params->next, args = args->next) {
	v = create_value_expression(args->val);
	local_env = putsym(local_env, VARIABLE, params->name, v, NULL);
  }
}

/* set symbolic_table to environment from stack */
void set_local() {
  local_env = pop_env();
}

void save_local() {
  push_env(local_env);
  init_table(local_env);
}

Symbol find_function(char* name) {
  return getsym(global_env, FUNCTION, name);
}

Symbol find_varivales(char* name) {
  Symbol s = getsym(local_env, VARIABLE, name);
  if (s == NULL) s = getsym(global_env, VARIABLE, name);
  return s;
}

/* arguments function */
Arguments generate_arg_list() {
  Arguments args = (Arguments)malloc(sizeof(*args));
  args->next = NULL;
  return args;
}

Arguments chain_param(Arguments args, char* name) {
  Arguments a = (Arguments)malloc(sizeof(*a));
  a->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
  strcpy(a->name, name);
  a->next = args;
  return a;
}

Arguments chain_arg(Arguments args, double val) {
  Arguments a = (Arguments)malloc(sizeof(*a));
  a->val = val;
  a->next = args;
  return a;
}

/* symbol functions */
Symbol putsym(Symbol env, ExpressionType type, char* name, Expression exp, Arguments params) {
  Symbol s = (Symbol)malloc(sizeof *s);
  s->name = (char *)malloc(sizeof(char)*(strlen(name)+1));
  s->type = type;
  strcpy(s->name, name);
  s->exp = exp;
  s->params = params;
  s->next = env;
  return s;
}

Symbol getsym(Symbol env, ExpressionType type, char* name) {
  Symbol s;
  for (s = env; s->next != NULL; s = s->next) {
	if (strcmp(s->name, name) == 0 && s->type == type) {
	  printf ("getsym:%s\n", s->name);
	  return s;
	}
  }
  return NULL;
}


/* int main(int argc, char *argv[]) { */
/*   init_env(); */
/*   Arguments args = generate_arg_list(); */
/*   Arguments params = generate_arg_list(); */

/*   params = chain_param(params, "n"); */
/*   args = chain_arg(args, 2); */
  
/*   Expression tree = create_function_expression("f", args); */
/*   printf ("Arguments:%s\n", tree->args->name); */

/*   Expression func = create_variable_expression("n"); */
/*   Expression v = create_value_expression(3.0); */
/*   Expression f = create_expression(ADD_EXPRESSION, func, v); */
  
/*   define_function("f", params, f); */
/*   Symbol test = find_function("f"); */
  
/*   printf ("aaa\n"); */
/*   printf("%f\n",eval(tree)); */
  
/*   return 0; */
/* } */