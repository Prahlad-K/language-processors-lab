%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "new_example.tab.h"

%}

DIGIT [0-9]+
FLOAT {DIGIT}\.{DIGIT}

%%

{DIGIT} {yylval.num = atoi(yytext);printf("Found a number %s\n",strdup(yytext));
        return DIGIT;}

{FLOAT} {yylval.point = atof(yytext);printf("Found a float %s\n", strdup(yytext));
        return FLOAT;}

" " {}


%%

