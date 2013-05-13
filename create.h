#define MAX_LENGTH 100

/* data structures */
typedef enum {
  VALUE,
  ADD_EXPRESSION,
  SUB_EXPRESSION,
  MUL_EXPRESSION,
  DIV_EXPRESSION,
  VARIABLE,
  FUNCTION
} ExpressionType;

typedef struct arguments_link* Arguments;
struct arguments_link{
  char* name;
  double val;
  Arguments next;
};

typedef struct expression_tree* Expression;
struct expression_tree{
  ExpressionType type;
  double val;
  char* identifier;
  Arguments args;
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
int pointer = 0;

/* extern Symbol local_env; */
/* extern Symbol global_env; */
/* extern Stack stack; */


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

/* eval functions */
double eval(Expression tree);

/* symbol functions */
Symbol putsym(Symbol env, ExpressionType type, char* name, Expression exp, Arguments params);
Symbol getsym(Symbol env, ExpressionType type, char* name);

/* function call sequence */
double function_call(char* name, Arguments args);
void save_local();
void set_local();
void set_arguments_to_local(Arguments params, Arguments args);

Symbol find_function(char* name);
Symbol find_varivales(char* name);

/* arguments functions */
Arguments generate_arg_list();
Arguments chain_param(Arguments args, char* name);
Arguments chain_arg(Arguments args, double val);

/* define functions */
void define_function(char* name, Arguments params, Expression exp);
void define_variable(char* name, Expression val);


