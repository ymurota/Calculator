#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "create.h"

/* global variables */
Symbol local_env;
Symbol global_env;
Stack stack;
int stack_pointer = 0;

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
  stack[stack_pointer++] = env;
}

Symbol pop_env() {
  return stack[--stack_pointer];
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
  Expression func = (Expression) malloc(sizeof *func);
  func->type = FUNCTION;
  func->identifier = (char*) malloc(sizeof(char)*(strlen(name)+1));
  strcpy(func->identifier, name);
  func->args = args;
  return func;
}

Expression create_if_expression(Condition cond, Expression lft, Expression rght) {
  Expression exp = (Expression) malloc(sizeof *exp);
  exp->type = IF_EXPRESSION;
  exp->cond = cond;
  exp->lft = lft;
  exp->rght = rght;
  return exp;
}

Condition create_condition(ConditionalOpe ope, Expression lft, Expression rght) {
  Condition cond = (Condition) malloc(sizeof *cond);
  cond->ope = ope;
  cond->lft = lft;
  cond->rght = rght;
  return cond;
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
	case POW_EXPRESSION:
	  return pow(eval(tree->lft), eval(tree->rght));
	case REM_EXPRESSION:
	  return (int)eval(tree->lft) % (int)eval(tree->rght);
	case FUNCTION:
	  return function_call(tree->identifier, tree->args);
	case VARIABLE:
	  s = find_variable(tree->identifier);
	  return eval(s->exp);
	case IF_EXPRESSION:
	  return eval_if(tree);
  case SIN_EXPRESSION:
    return sin(eval(tree->lft));
  case COS_EXPRESSION:
    return cos(eval(tree->lft));
	case TAN_EXPRESSION:
	  return tan(eval(tree->lft));
	}
  } else {
	return tree->val;
  }
}

double eval_if(Expression tree) {
  if (eval_bool(tree->cond)) {
	return eval(tree->lft);
  } else {
	return eval(tree->rght);
  }
}

int eval_bool(Condition cond) {
  double lft = eval(cond->lft);
  double rght = eval(cond->rght);
  switch (cond->ope) {
  case EQUAL:
	return lft == rght;
  case NOT_EQUAL:
	return lft != rght;
  case LESS:
	return lft < rght;
  case GREATER:
	return lft > rght;
  case LESS_EQUAL:
	return lft <= rght;
  case GREATER_EQUAL:
	return lft >= rght;
  }
}

/* define functions */
void define_function(char* name, Arguments params, Expression exp) {
  global_env = putsym(global_env, FUNCTION, name, exp, params);
}

void define_variable(char* name, Expression exp) {
  global_env = putsym(global_env, VARIABLE, name, exp, NULL);
  Symbol a = find_variable(name);
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
	v = create_value_expression(eval(args->exp));
	local_env = putsym(local_env, VARIABLE, params->name, v, NULL);
  }
}

/* set symbolic_table from stack to environment */
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

Symbol find_variable(char* name) {
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

Arguments chain_arg(Arguments args, Expression exp) {
  Arguments a = (Arguments)malloc(sizeof(*a));
  //a->val = val;
  a->exp = exp;
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
	  return s;
	}
  }
  return NULL;
}
