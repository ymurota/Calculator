%{
#include <stdio.h>
#include "y.tab.h"
#include "create.h"
  
 int
yywrap(void)
{
	return 1;
}
%}
%%
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

%%
