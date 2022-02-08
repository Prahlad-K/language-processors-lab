
%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct exprType{

	char *addr;
	char *code;
	char *type;  
};


struct symtab
{
	char* name;
	char* type;
	struct symtab* ptr;
};

int n=1;
int nl = 1;
char *var;
char num_to_concatinate[10];
char num_to_concatinate_l[10];
char *ret;
char *temp;

char *label;
char *label2;
char *check;

char *begin;

char *b1;
char *b2;

char *s1;
char *s2;

struct symtab* S[10];
int symind =0;

struct exprType *to_return_expr;

char * newTemp(){
	
	char *newTemp = (char *)malloc(20);
	strcpy(newTemp,"t");
	snprintf(num_to_concatinate, 10,"%d",n);
	strcat(newTemp,num_to_concatinate);
		
	n++;
	return newTemp;
}

char * newLabel(){
	
	char *newLabel = (char *)malloc(20);
	strcpy(newLabel,"10");
	snprintf(num_to_concatinate_l, 10,"%d",nl);
	strcat(newLabel,num_to_concatinate_l);
		
	nl++;
	return newLabel;
}

struct symtab* newentry(char* name, char* type, struct symtab* ptr)
{
	
	struct symtab* N =malloc(100);
	N->name = malloc(100);
	N->type = malloc(100);	
	strcpy(N->name, name);
	strcpy(N->type, type);
	N->ptr = ptr;
	return N;
}

void addentry(struct symtab* s)
{
	S[symind++] = s;
}

struct symtab* lookup(char* name)
{

	for(int i=0;i<symind;i++)
	{
		if(strcmp(S[i]->name, name)==0)
		{
			return S[i];
		}	
 	}
	return NULL;
}

void printSymbolTable()
{
	for(int i=0;i<symind;i++)
	{
		printf("Entry: %s %s \n", S[i]->name, S[i]->type);
	}
}

%}

%start startSym

%union {
	int ival;
	float fval;
	char *sval;
	struct exprType *EXPRTYPE;
}
%token <ival> DIGIT
%token <fval> FLOAT
%token <sval> ID IF ELSE WHILE TYPES REL_OPT OR AND NOT TRUE FALSE ADDAS SUBAS MULAS DIVAS FOR
%token <sval> '+' '-' '*' '/' '^' '%' '\n' '=' ';'
%type <sval> list text construct block dec bool program startSym
%type <EXPRTYPE> expr number stat instat

%left OR
%left AND
%left NOT
%left REL_OPT
%right '='
%left '+' '-'
%left '*' '/' '%'
%right '^'

%%

startSym:	program
		{
			s1 = $1;
			label = newLabel();

			check = strstr (s1,"NEXT");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (s1,"NEXT");
				}

			ret = (char *)malloc(strlen(s1)+110);
			ret[0] = 0;

			strcat(ret,s1);
			strcat(ret,"\n");
			strcat(ret,label);
			strcat(ret," : END\n");
			
			//printf("\n----------  FINAL THREE ADDRESS CODE ----------\n");
			puts(ret);

			$$ = ret;
		}
		;

program : 	program construct
		{

			s1 = $1;
			s2 = $2;

			label = newLabel();

			check = strstr (s1,"NEXT");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (s1,"NEXT");
				}

			ret = (char *)malloc(strlen($1)+strlen($2)+14);
			ret[0] = 0;
			strcat(ret,$1);
			strcat(ret,"\n");
			strcat(ret,label);
			strcat(ret," : ");
			strcat(ret,$2);

			printf("program construct\n");

			puts(ret);
			$$ = ret;
		}
		|
		construct
		{
			printf("Final Construct \n");
			puts($1);
			$$ = $1;
		}
		|
		list
		{	
			printf("Final list \n");
			puts($1);
			$$ = $1;
		}
		;

construct :     block
		{
			$$ = $1;
		}
		|
		WHILE '(' bool ')' block
		{
			printf("Inside WHILE\n");
			puts($5);
			
			b1 = $3;
			s1 = $5;

			begin = newLabel();
			label = newLabel();

			check = strstr (b1,"TRUE");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TRUE");
				}

			check = strstr (b1,"FAIL");
			
			while(check!=NULL){
				strncpy (check,"NEXT",4);
				//strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"FAIL");
				}

			check = strstr (s1,"NEXT");
			
			while(check!=NULL){
				strncpy (check,begin,strlen(begin));
				strncpy (check+strlen(begin),"    ",(4-strlen(begin)));
				check = strstr (s1,"NEXT");
				}

			ret = (char *)malloc(strlen(b1)+strlen(s1)+120);
			ret[0] = 0;
			strcat(ret,begin);
			strcat(ret," : ");
			strcat(ret,b1);
			strcat(ret,"\n");
			strcat(ret,label);
			strcat(ret," : ");
			strcat(ret,s1);

			strcat(ret,"\n");
			strcat(ret,"goto ");
			strcat(ret,begin);
			
			printf("Final return from while\n");
			puts(ret);

			$$ = ret;
	
		}
		|
		IF '(' bool ')' block
		{
			printf("Inside IF\n");
			
			label = newLabel();
			b1 = $3;

			check = strstr (b1,"TRUE");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TRUE");
				}
			
			check = strstr (b1,"FAIL");
			
			while(check!=NULL){
				strncpy (check,"NEXT",4);
				//strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"FAIL");
				}

			ret = (char *)malloc(strlen(b1)+strlen($5)+14);
			ret[0] = 0;
			strcat(ret,b1);
			strcat(ret,"\n");
			strcat(ret,label);
			strcat(ret," : ");
			strcat(ret,$5);
			
			puts(ret);
			$$ = ret;
		}
		|
		IF '(' bool ')' block ELSE block
		{
			printf("Inside if then else\n");

			b1 = $3;
			label = newLabel();

			check = strstr (b1,"TRUE");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TRUE");
				}
			

			label2 = newLabel();
			check = strstr (b1,"FAIL");

			while(check!=NULL){
				strncpy (check,label2,strlen(label2));
				strncpy (check+strlen(label2),"    ",(4-strlen(label2)));
				check = strstr (b1,"FAIL");
				}

			ret = (char *)malloc(strlen(b1)+strlen($5)+strlen($7)+120);
			ret[0] = 0;
			strcat(ret,b1);
			strcat(ret,"\n");
			strcat(ret,label);
			strcat(ret," : ");
			strcat(ret,$5);
			strcat(ret,"\n");
			strcat(ret,"goto NEXT");
			strcat(ret,"\n");
			strcat(ret,label2);
			strcat(ret," : ");
			strcat(ret,$7);
			
			puts(ret);

			$$ = ret;
	
		}
		|
		FOR '(' stat bool ';' instat ')' block
		{
			printf("Inside FOR\n");

		
			puts($8);
			
			b1 = $4;
			s1 = $8;

			begin = newLabel();
			label = newLabel();

			check = strstr (b1,"TRUE");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TRUE");
				}

			check = strstr (b1,"FAIL");
			
			while(check!=NULL){
				strncpy (check,"NEXT",4);
				//strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"FAIL");
				}

			check = strstr (s1,"NEXT");
			
			while(check!=NULL){
				strncpy (check,begin,strlen(begin));
				strncpy (check+strlen(begin),"    ",(4-strlen(begin)));
				check = strstr (s1,"NEXT");
				}

			ret = (char *)malloc(strlen($3->code)+strlen(b1)+strlen(s1)+strlen($6->code)+20);
			ret[0] = 0;
			strcat(ret, $3->code);
			strcat(ret, "\n");
			strcat(ret,begin);
			strcat(ret," : ");
			strcat(ret,b1);
			strcat(ret,"\n");
			strcat(ret,label);
			strcat(ret," : ");
			strcat(ret,s1);
			strcat(ret,"\n");
			strcat(ret, $6->code);
			strcat(ret, "\n");
			strcat(ret,"goto ");
			strcat(ret,begin);
			
			printf("Final return from for\n");
			puts(ret);

			$$ = ret;
		}
		|
		bool '?' block ':' block
		{
			printf("Ternary operations!\n");
			b1 = $1;
			label = newLabel();

			check = strstr (b1,"TRUE");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TRUE");
				}
			

			label2 = newLabel();
			check = strstr (b1,"FAIL");

			while(check!=NULL){
				strncpy (check,label2,strlen(label2));
				strncpy (check+strlen(label2),"    ",(4-strlen(label2)));
				check = strstr (b1,"FAIL");
				}

			ret = (char *)malloc(strlen(b1)+strlen($3)+strlen($5)+110);
			ret[0] = 0;
			strcat(ret,b1);
			strcat(ret,"\n");
			strcat(ret,label);
			strcat(ret," : ");
			strcat(ret,$3);
			strcat(ret,"\n");
			strcat(ret,"goto NEXT");
			strcat(ret,"\n");
			strcat(ret,label2);
			strcat(ret," : ");
			strcat(ret,$5);
			
			puts(ret);

			$$ = ret;
			
		}

		;

block:		'{' list '}'
		{
			printf("Inside block\n");
			$$ = $2;
		}
		|
		'{' construct '}'
		{
			$$ = $2;
		}
		;
	 

list:    stat               /* Base Condition */
		{
			$$ = $1->code;
		}
         |
        list stat
		{
			ret = (char *)malloc(strlen($1)+strlen($2->code)+4);
			ret[0] = 0;

			strcat(ret,$1);
			strcat(ret,"\n");
			strcat(ret,$2->code);
		
			printf("Inside list stat \n");
			puts(ret);
			$$ = ret;
		}
	 |
        list error '\n'
         {
           yyerrok;
         }
         ;

stat: instat ';'
	{
		$$ = $1;
	 }
;

instat:   
	 {
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		
		to_return_expr->code = (char *)malloc(2);
		to_return_expr->code[0] = 0;
		
		$$ = to_return_expr;
	 }
	 |
	 expr 
         {
		$$ = $1;
           	
         }
	 |
	 dec 
         {
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = $1;
		
		to_return_expr->code = (char *)malloc(2);
		to_return_expr->code[0] = 0;
		
		$$ = to_return_expr;
           
         }
         |
         text '=' expr 
         {
	    printf("Assignment statement \n");

		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
		/*
		if(lookup($1)==NULL)
		printf("Variable %s undeclared!\n", $1);

		if(strcmp(lookup($1)->type, $3->type)!=0)
		{
			if(strcmp(lookup($1)->type, "int")==0)
			{
				char* res = malloc(100);
				strcat(res, "to_int(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
			else
			{
				char* res = malloc(100);
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
		}
		*/
		strcat(ret,$1);

		strcat(ret,"=");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
	    
			
		//printf(" %s = %s \n",$1,$3->addr);
          
	    
         }
	|
         text ADDAS expr 
         {
	    printf("Plus Assignment statement \n");

		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
/*
		if(strcmp(lookup($1)->type, $3->type)!=0)
		{
			if(strcmp(lookup($1)->type, "int")==0)
			{
				char* res = malloc(100);
				strcat(res, "to_int(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
			else
			{
				char* res = malloc(100);
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
		}
*/
		strcat(ret,$1);

		strcat(ret,"=");
		strcat(ret,$1);	
		strcat(ret, "+");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
	    
			
		//printf(" %s = %s \n",$1,$3->addr);
          
	    
         }
	|
         text SUBAS expr 
         {
	    printf("Minus Assignment statement \n");

		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
		/*
		if(strcmp(lookup($1)->type, $3->type)!=0)
		{
			if(strcmp(lookup($1)->type, "int")==0)
			{
				char* res = malloc(100);
				strcat(res, "to_int(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
			else
			{
				char* res = malloc(100);
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
		}
		*/
		strcat(ret,$1);

		strcat(ret,"=");
		strcat(ret,$1);	
		strcat(ret, "-");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
	    
			
		//printf(" %s = %s \n",$1,$3->addr);
          
	    
         }
	|
         text MULAS expr 
         {
	    printf("Multiplication Assignment statement \n");

		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
		/*
		if(strcmp(lookup($1)->type, $3->type)!=0)
		{
			if(strcmp(lookup($1)->type, "int")==0)
			{
				char* res = malloc(100);
				strcat(res, "to_int(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
			else
			{
				char* res = malloc(100);
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
		}
		*/
		strcat(ret,$1);

		strcat(ret,"=");
		strcat(ret,$1);	
		strcat(ret, "*");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;


		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
	    
			
		//printf(" %s = %s \n",$1,$3->addr);
          
	    
         }
	|
         text DIVAS expr 
         {
	    printf("Division Assignment statement \n");

		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
/*
		if(strcmp(lookup($1)->type, $3->type)!=0)
		{
			if(strcmp(lookup($1)->type, "int")==0)
			{
				char* res = malloc(100);
				strcat(res, "to_int(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
			else
			{
				char* res = malloc(100);
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
		}
*/
		strcat(ret,$1);

		strcat(ret,"=");
		strcat(ret,$1);	
		strcat(ret, "/");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;

		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
	    
			
		//printf(" %s = %s \n",$1,$3->addr);
          
	    
         }
	 |
	 dec '=' expr
         {
	    printf("Dec and Assignment statement \n");
	    
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
	/*
		if(strcmp(lookup($1)->type, $3->type)!=0)
		{
			if(strcmp(lookup($1)->type, "int")==0)
			{
				char* res = malloc(100);
				strcat(res, "to_int(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
			else
			{
				char* res = malloc(100);
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}
		}
		//strcat(ret,to_return_expr->addr);
		*/
		strcat(ret,$1);
		strcat(ret,"=");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
			
		//printf(" %s = %s \n",$1,$3->addr);
          
	    
         }
         ;

dec : 		TYPES text
		{	
			
			printf("Inside Decleration!\n");
			struct symtab* s = lookup($2);
			if(s!=NULL)
			{
				printf("Already defined variable %s\n", $2);
			}
			else
			{	
				struct symtab* s1 = newentry($2, $1, NULL);
				addentry(s1);
			}
			//printSymbolTable();
			$$ = $2;	
		}
		;

bool : 	 	expr REL_OPT expr
		{
			printf("Inside rel opt\n");

			temp = (char *)malloc(strlen($1->code)+strlen($3->code)+50);
			temp[0] = 0;
	
			if($1->code[0]!=0){
				strcat(temp,$1->code);
				strcat(temp,"\n");
				}
			if($3->code[0]!=0){
				strcat(temp,$3->code);
				strcat(temp,"\n");
				}

			ret = (char *)malloc(50);
			ret[0] = 0;
			strcat(ret,"if(");
			strcat(ret,$1->addr);
			strcat(ret,$2);
			strcat(ret,$3->addr);
			strcat(ret,") goto TRUE \n goto FAIL");

			strcat(temp,ret);

			$$ = temp;
		}
		|
		bool OR bool
		{
			printf("Inside OR\n");
			
			b1 = $1;
			b2 = $3;

			label = newLabel();

			check = strstr (b1,"FAIL");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"FAIL");
				}
			
			temp = (char *)malloc(strlen(b1)+strlen(b2)+10);
			temp[0] = 0;

			strcat(temp,b1);
			strcat(temp,"\n");
			strcat(temp,label);
			strcat(temp," : ");
			strcat(temp,b2);

			$$ = temp;
		}
		|
		bool AND bool
		{
			printf("Inside AND\n");

			b1 = $1;
			b2 = $3;

			label = newLabel();

			check = strstr (b1,"TRUE");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TRUE");
				}
			
			temp = (char *)malloc(strlen(b1)+strlen(b2)+10);
			temp[0] = 0;

			strcat(temp,b1);
			strcat(temp,"\n");
			strcat(temp,label);
			strcat(temp," : ");
			strcat(temp,b2);

			$$ = temp;
		}
		|
		NOT '(' bool ')'
		{
			printf("Inside NOT\n");
			puts($3);

			b1 = $3;

			label = "TEFS";

			check = strstr (b1,"TRUE");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				//strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TRUE");
				}
			
			label = "TRUE";
			check = strstr (b1,"FAIL");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				//strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"FAIL");
				}

			label = "FAIL";
			check = strstr (b1,"TEFS");
			
			while(check!=NULL){
				strncpy (check,label,strlen(label));
				//strncpy (check+strlen(label),"    ",(4-strlen(label)));
				check = strstr (b1,"TEFS");
				}
			
			$$ = b1;
		}
		|
		'(' bool ')'
		{
			$$ = $2;
		}
		|
		TRUE
		{
			printf("Inside TRUE\n");

			ret = (char *)malloc(20);
			ret[0] = 0;
			strcat(ret,"\ngoto TRUE");
			
			$$ = ret;
		}
		|
		FALSE
		{
			printf("Inside FALSE\n");

			ret = (char *)malloc(20);
			ret[0] = 0;
			strcat(ret,"\ngoto FAIL");
			
			$$ = ret;
		}
		;

expr:    '(' expr ')'
         {
           $$ = $2;
         }
         |
	 expr '^' expr
         {
		printf("Exponential : ");
		
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->type = malloc(100);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
		
		if(strcmp($1->type, $3->type)==0)
		{
			strcpy(to_return_expr->type, $1->type);
		}
		else
		{
			strcpy(to_return_expr->type, "float");
			char* res = malloc(100);
			if(strcmp($1->type, "int")==0)
			{
				strcat(res, "to_float(");
				strcat(res, $1->addr);
				strcat(res, ")");
				strcpy($1->addr, res);
			}
			else
			{
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}

		}
		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"^");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
	
         }
	 |
         expr '*' expr
         {

           printf("Multiplication : ");
	   	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->type = malloc(100);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
		if(strcmp($1->type, $3->type)==0)
		{
			strcpy(to_return_expr->type, $1->type);
		}
		else
		{
			strcpy(to_return_expr->type, "float");
			char* res = malloc(100);
			if(strcmp($1->type, "int")==0)
			{
				strcat(res, "to_float(");
				strcat(res, $1->addr);
				strcat(res, ")");
				strcpy($1->addr, res);
			}
			else
			{
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}

		}
		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"*");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
           
         }
         |
         expr '/' expr
         {
           printf("Division : ");
	  	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->type = malloc(100);
		to_return_expr->addr = newTemp();
		
		ret = (char *)malloc(20);
		ret[0] = 0;
		if(strcmp($1->type, $3->type)==0)
		{
			strcpy(to_return_expr->type, $1->type);
		}
		else
		{
			strcpy(to_return_expr->type, "float");
			char* res = malloc(100);
			if(strcmp($1->type, "int")==0)
			{
				strcat(res, "to_float(");
				strcat(res, $1->addr);
				strcat(res, ")");
				strcpy($1->addr, res);
			}
			else
			{
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}

		}

		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"/");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
	   
         }
         |
         expr '%' expr
         {
           printf("Modulo Division : \n");
	   	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(120);
		to_return_expr->type = malloc(100);
		to_return_expr->addr = newTemp();
		 	
		ret = (char *)malloc(120);
		ret[0] = 0;

		printf("D\n");
		if(strcmp($1->type, $3->type)==0)
		{
			printf("D\n");
			strcpy(to_return_expr->type, $1->type);

		}
		else
		{	
			printf("D2\n");
			strcpy(to_return_expr->type, "float");
			char* res = malloc(100);
			if(strcmp($1->type, "int")==0)
			{
				strcat(res, "to_float(");
				strcat(res, $1->addr);
				strcat(res, ")");
				strcpy($1->addr, res);
			}
			else
			{
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}

		}
		
		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"%");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;

         }
         |
         expr '+' expr
         {
           	printf("Addition : ");
	   	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->type = malloc(100);
		to_return_expr->addr = newTemp();

		ret = (char *)malloc(20);
		ret[0] = 0;
		if(strcmp($1->type, $3->type)==0)
		{
			strcpy(to_return_expr->type, $1->type);
		}
		else
		{
			strcpy(to_return_expr->type, "float");
			char* res = malloc(100);
			if(strcmp($1->type, "int")==0)
			{
				strcat(res, "to_float(");
				strcat(res, $1->addr);
				strcat(res, ")");
				strcpy($1->addr, res);
			}
			else
			{
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}

		}
		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"+");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);

		to_return_expr->code = temp;

           	$$ = to_return_expr;
         }
         |
         expr '-' expr
         {
	   printf("Subtraction : ");
           	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->type = malloc(100);
		to_return_expr->addr = newTemp();

		ret = (char *)malloc(20);
		ret[0] = 0;

		if(strcmp($1->type, $3->type)==0)
		{
			strcpy(to_return_expr->type, $1->type);
		}
		else
		{
			strcpy(to_return_expr->type, "float");
			char* res = malloc(100);
			if(strcmp($1->type, "int")==0)
			{
				strcat(res, "to_float(");
				strcat(res, $1->addr);
				strcat(res, ")");
				strcpy($1->addr, res);
			}
			else
			{
				strcat(res, "to_float(");
				strcat(res, $3->addr);
				strcat(res, ")");
				strcpy($3->addr, res);
			}

		}
		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"-");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);
		
		to_return_expr->code = temp;

           	$$ = to_return_expr;
		
         }
	|
	 expr '&' expr
         {
	   	printf("Binary AND : ");
           	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();

		ret = (char *)malloc(20);
		ret[0] = 0;

		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"&");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);
		
		to_return_expr->code = temp;

           	$$ = to_return_expr;
		
         }
	|
	expr '~' expr
         {
	   printf("Binary NOT : ");
           	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();

		ret = (char *)malloc(20);
		ret[0] = 0;

		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"~");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);
		
		to_return_expr->code = temp;

           	$$ = to_return_expr;
		
         }
	|
	expr '|' expr
         {
	   printf("Binary OR : ");
           	to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->addr = newTemp();

		ret = (char *)malloc(20);
		ret[0] = 0;

		strcat(ret,to_return_expr->addr);

		strcat(ret,"=");
		strcat(ret,$1->addr);
		strcat(ret,"|");
		strcat(ret,$3->addr);
		printf("RET  = \n");
		puts(ret);

		temp = (char *)malloc(strlen($1->code)+strlen($3->code)+strlen(ret)+6);

		temp[0] = 0;
		
		if ($1->code[0]!=0){
			strcat(temp,$1->code);
			strcat(temp,"\n");
			}
		if ($3->code[0]!=0){
			strcat(temp,$3->code);
			strcat(temp,"\n");
			}
		strcat(temp,ret);
		printf("TEMP = \n");

		puts(temp);
		
		to_return_expr->code = temp;

           	$$ = to_return_expr;
		
         }
         |
	 text {
		printf("Inside text %s\n", $1);
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(20);
		to_return_expr->type = malloc(100);
		to_return_expr->addr = $1;
		
		struct symtab* s;
		if(s=lookup($1))
			strcpy(to_return_expr->type, s->type);						
		else
			printf("ERROR, variable %s not declared!", $1);

		to_return_expr->code = (char *)malloc(2);
		to_return_expr->code[0] = 0;

		$$ = to_return_expr;
		}
         |
         number {
		printf("Inside Number\n");
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(120);
		to_return_expr->addr = $1->addr;
		
		to_return_expr->code = (char *)malloc(20);
		to_return_expr->code = "";

		to_return_expr->type = malloc(100);
		to_return_expr->type = $1->type;


		$$ = to_return_expr;}
         ;

text: 	ID
         {
		printf("Inside Identifier : %s\n",$1);
           	$$ = $1;
         }
	  ;

number:  DIGIT
         {
		printf("Inside DIGIT : %d\n",$1);
		var = (char *)malloc(20);
        snprintf(var, 10,"%d",$1);
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(120);

		strcpy(to_return_expr->addr, var);


        to_return_expr->type = malloc(100);
        strcpy(to_return_expr->type, "int");

        $$ = to_return_expr;
         } 
	 |
         FLOAT
         {
		printf("Inside FLOAT : %d\n",$1);
		var = (char *)malloc(20);
        snprintf(var, 10,"%f",$1);
		to_return_expr = (struct exprType *)malloc(sizeof(struct exprType));
		to_return_expr->addr = (char *)malloc(120);

		strcpy(to_return_expr->addr, var);


        to_return_expr->type = malloc(100);
        strcpy(to_return_expr->type, "float");

        $$ = to_return_expr;
           
         } 
	;
	
%%

extern int yylex();
extern int yyparse();
extern FILE *yyin;

main() {
	// open a file handle to a particular file:
	FILE *myfile = fopen("new_input.txt", "r");
	// make sure it is valid:
	if (!myfile) {
		printf("Error opening file!");
		return -1;
	}
	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
	
}

void yyerror(const char *s) {
	printf("Parse Error: ");
	puts(s);
	//printf("\n");
	// might as well halt now:
	exit(-1);
}
