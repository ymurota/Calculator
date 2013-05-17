%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "create.h"
 int
yywrap(void)
{
	return 1;
}
%}
%%
"def"   return DEF;
","     return COMMA;
"="     return EQ;
"("     return LP;
")"     return RP;
"+"		return ADD;
"-"		return SUB;
"*"		return MUL;
"/"		return DIV;
"^"     return POW;
"%"     return REM;
"\n"	return CR;
"?"     return IF;
":"     return COLON;
"=="    return EQL;
"!="    return N_EQL;
"<"     return LS;
">"     return GR;
">="    return LS_E;
"<="    return GR_E;
"sin"   return SIN

[1-9][0-9]* {
  double tmp;
  sscanf(yytext, "%lf", &tmp);
  Expression val = create_value_expression(tmp);
  yylval.val = val;
  return NUMBER;
}
[0-9]*\.[0-9]* {
  double tmp;
  sscanf(yytext, "%lf", &tmp);
  Expression val = create_value_expression(tmp);
  yylval.val = val;
  return NUMBER;
}
[a-zA-Z][a-zA-Z0-9]* {
  char tmp[100];
  sscanf(yytext, "%s", tmp);
  strcpy(yylval.name, tmp);
  return IDENTIFIER;
}
%%
