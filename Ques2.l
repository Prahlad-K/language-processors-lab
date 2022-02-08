/*  Program to encrypt the text using lex */

%{
typedef struct node
{
	char* data;
	struct node* left;
	struct node* right;
}*BTPTR;

typedef struct stack
{
	BTPTR ptr[100];
	int top;
}*SPTR;

void push(SPTR s, BTPTR node);
BTPTR pop(SPTR s);
BTPTR top(SPTR s);
BTPTR newnode(char* data);
SPTR newstack();
int isOperator(char* op);
int preIndex(char* op, char* p);
int higher(char* op1, char* op2, char* p);
void postorder(BTPTR T);

void build(char c, SPTR soptr, SPTR sopnd);

SPTR soptr, sopnd;
BTPTR root;
char* p;

%}

%%

[0-9a-bA-B+\-*/@~ ] {build(*yytext, soptr, sopnd);}

\n {root = pop(sopnd); postorder(root);printf("\n");}
. 
 
%%

void push(SPTR s, BTPTR node)
{
	
	s->ptr[++s->top] = node;
}

BTPTR pop(SPTR s)
{
	if(s->top==-1)
	return NULL;

	return s->ptr[s->top--];
}

BTPTR top(SPTR s)
{
	if(s->top==-1)
	return NULL;
	return s->ptr[s->top];
}

BTPTR newnode(char* data)
{
	BTPTR N = (BTPTR)malloc(100);
	N->data = malloc(100);
	
	strcpy(N->data, data);
	N->left = NULL;
	N->right = NULL;
	return N;
}

SPTR newstack()
{
	SPTR s;
	s = malloc(100);
	s->top= -1;
	return s;
}

int isOperator(char* op)
{
	if(*op=='@'||*op=='/'||*op=='*'||*op=='+'||*op=='-'||*op==' '||*op=='~')
	return 1;
	else
	return 0;
}

int preIndex(char* op, char* p)
{
	for(int i=0;i<7;i++)
	{
	if(p[i]==*op)
	return i;
	}
}


int higher(char* op1, char* op2, char* p)
{

	if(preIndex(op1, p)<preIndex(op2, p))
	return 1;
	else
	return 0;
}


void postorder(BTPTR T)
{
	if(T)
	{
	postorder(T->left);
	postorder(T->right);

	printf("%s", T->data);
	}
}


void build(char c, SPTR soptr, SPTR sopnd)
{

	char* op = malloc(2);
	op[0] = c;

	label:
	if(isOperator(op))
	{
		BTPTR tos = top(soptr);
		if(tos&&higher(tos->data, op, p))
		{
			//pop the operator and attach operands to it.
			if(*op!='~')
			{
				pop(soptr);

				BTPTR a = pop(sopnd);
				BTPTR b = pop(sopnd);
				BTPTR N = newnode(tos->data);
				N->left = b;
				N->right = a;

				push(sopnd, N);
			}
			else
			{
				pop(soptr);

				BTPTR a = pop(sopnd);

				BTPTR N = newnode(tos->data);
				N->left = NULL;
				N->right = a;

				push(sopnd, N);
			}
			goto label;

		}
		else
		{
			//stack is in decreading order of precedence!
			BTPTR N = newnode(op);
			push(soptr, N);
		}

	}
	else
	{
		BTPTR N = newnode(op);
		push(sopnd, N);
	}

}
main(int argc, char** argv)
{

p = malloc(100);

p[0] = '~';
p[1] = '@';
p[2] = '/';
p[3] = '*';
p[4] = '+';
p[5] = '-';
p[6] = ' ';

//precedence

soptr = newstack();
//for operators
sopnd = newstack();
//for operands
yylex();


return 0;
}


