%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

double ans = 0;

%}

%union {
    int num;
    float point;
}
%start S

%token <num> DIGIT 
%token <point> FLOAT

%type <point> S

%%

S : DIGIT
    {
        printf("Found last digit %d\n", $1);
        ans = ans + $1;
    }
    |
    FLOAT
    {
        printf("Found last float %f\n", $1);
        ans = ans + $1;
    }
    ;
%%

int main()
{
    yyparse();

    printf("Answer is %f\n", ans);
}

void yyerror(const char *s) {
	printf("Parse Error: ");
	puts(s);
	//printf("\n");
	// might as well halt now:
	exit(-1);
}