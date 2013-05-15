#define MAX_LENGTH 100
/* data structures */
typedef enum {
  VALUE,
  ADD_EXPRESSION,
  SUB_EXPRESSION,
  MUL_EXPRESSION,
  DIV_EXPRESSION,
  POW_EXPRESSION,
  REM_EXPRESSION,
  VARIABLE,
  FUNCTION,
  IF_EXPRESSION,
} ExpressionType;

typedef enum {
  EQUAL, // ==
  NOT_EQUAL, // !=
  LESS, // <
  GREATER, // >
  LESS_EQUAL, // <=
  GREATER_EQUAL // >=
} ConditionalOpe;

typedef struct arguments_link* Arguments;
struct arguments_link{
  char* name;
  double val;
  Arguments next;
};

typedef struct condition* Condition;
typedef struct expression_tree* Expression;

struct condition{
  Expression lft;
  Expression rght;
  ConditionalOpe ope;
};

struct expression_tree{
  ExpressionType type;
  double val;
  char* identifier;
  Arguments args;
  Condition cond;
  Expression lft;
  Expression rght;
};

typedef struct symbol* Symbol;
struct symbol{
  ExpressionType type;
  char* name;
  Arguments params;
  Expression exp;
  Symbol next;
};

typedef Symbol Stack[MAX_LENGTH];

/* environment functions */
Symbol init_table(Symbol table);
void init_env();
void push_env(Symbol env);
Symbol pop_env();

/* create functions */
Expression create_expression(ExpressionType type, Expression lft, Expression rght);
Expression create_value_expression(double val);
Expression create_function_expression(char* name, Arguments args);
Expression create_variable_expression(char* name);
Expression create_if_expression(Condition cond, Expression lft, Expression rght);
Condition create_condition(ConditionalOpe ope, Expression lft, Expression rght);

/* eval functions */
double eval(Expression tree);
double eval_if(Expression tree);
int eval_bool(Condition cond);

/* symbol functions */
Symbol putsym(Symbol env, ExpressionType type, char* name, Expression exp, Arguments params);
Symbol getsym(Symbol env, ExpressionType type, char* name);

/* function call sequence */
double function_call(char* name, Arguments args);
void save_local();
void set_local();
void set_arguments_to_local(Arguments params, Arguments args);

Symbol find_function(char* name);
Symbol find_variable(char* name);

/* arguments functions */
Arguments generate_arg_list();
Arguments chain_param(Arguments args, char* name);
Arguments chain_arg(Arguments args, double val);

/* define functions */
void define_function(char* name, Arguments params, Expression exp);
void define_variable(char* name, Expression val);
