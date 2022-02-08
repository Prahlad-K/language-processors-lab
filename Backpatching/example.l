%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

char c;

%}

DIGIT  [0-9]+
ID [a-zA-Z_][0-9a-zA-Z_]*
FLOAT [0-9]+[.][0-9]*

ARITHMETIC_OPERATORS ['+' '\-' '*' '/' '^' '%']
OTHER_CHARACTERS ["=" ";" "(" ")" "{" "}"  "-=" "*=" "/="]
UNARY_OPERATORS ['&' '|' '~']
TERNARY_OPERATORS ['?' ':']

ADDAS ["+="]
SUBAS ["-="]
MULAS ["*="]
DIVAS ["/="]

TYPES ["int" "float" "long" "char" "double"]

%%
[ \t] ;
[\n] ;


"if" 	{ return (IF); }
"else" 	{ return (ELSE); }
"while"	{ return (WHILE); }
"for"	{ return (FOR);	}

"int"	{ yylval.sval = strdup(yytext);return (TYPES); }
"float"	{ yylval.sval = strdup(yytext);return (TYPES); }
"long"	{ yylval.sval = strdup(yytext);return (TYPES); }
"char"	{ yylval.sval = strdup(yytext);return (TYPES); }
"double"	{ return (TYPES); }

">"	{yylval.sval = strdup(yytext);
		return (REL_OPT);}
"<"	{yylval.sval = strdup(yytext);
		return (REL_OPT);}
">="	{yylval.sval = strdup(yytext);
		return (REL_OPT);}
"<="	{yylval.sval = strdup(yytext);
		return (REL_OPT);}
"!="	{yylval.sval = strdup(yytext);
		return (REL_OPT);}
"=="	{yylval.sval = strdup(yytext);
		return (REL_OPT);}

"+="	{yylval.sval = strdup(yytext);
		return (ADDAS);}
"-="	{yylval.sval = strdup(yytext);
		return (SUBAS);}
"*="	{yylval.sval = strdup(yytext);
		return (MULAS);}
"/="	{yylval.sval = strdup(yytext);
		return (DIVAS);}


"||"	{yylval.sval = strdup(yytext);
		return (OR);}
"&&"	{yylval.sval = strdup(yytext);
		return (AND);}
"!"	{yylval.sval = strdup(yytext);
		return (NOT);}
"true"	{yylval.sval = strdup(yytext);
		return (TRUE);}
"false"	{yylval.sval = strdup(yytext);
		return (FALSE);}

{DIGIT}  { yylval.ival = atoi(yytext);
	   return DIGIT; }

{FLOAT}  { yylval.fval = atof(yytext);
	   return FLOAT; }

{ID} {
	    yylval.sval = strdup(yytext);
	    return ID; 
}

{TERNARY_OPERATORS}	{
		c = yytext[0];
		return (c);
		}

{UNARY_OPERATORS}	{
		c = yytext[0];
		return (c);
		}

{OTHER_CHARACTERS}   {
                 c = yytext[0];
                 return(c);
              }
{ARITHMETIC_OPERATORS}   {
            	c = yytext[0];
                 return(c);
              }



%%
