/* program to generate a symbol table using words in a given English text */

%option noyywrap nodefault yylineno case-insensitive


/* declaring the symbol table */

%{

struct symbol
{
char* name;
struct ref* reflist;
int freq;
};

struct ref
{
int lineno;
struct ref* next;
char* filename;
int flags;
};

#define NHASH 9997

struct symbol symtab[NHASH];

struct symbol* lookup(char*);

void addref(int , char*, char*, int);

char* curfilename; 

%}

%%

a |
an |
and |
are |
as |
at |
be |
but |
for |
in |
is |
it |
of |
on |
or |
that |
the |
this |
to

[a-z]+(\'(s|t))? { addref(yylineno, curfilename, yytext, 0); }
.|\n 
%%

/* main routine */

main(int argc, char** argv)
{
	int i;
	if(argc<2)
	{
	curfilename = "stdin";
	yylineno = 1;
	yylex();
	}
	else
	{for(int i=1;i<argc;i++)
	{
	FILE* f = fopen(argv[i], "r");

	curfilename = argv[i];
	yyrestart(f);
	yylineno = 1;
	yylex();
	fclose(f);
	}
	}	

printrefs();
}

static unsigned symhash(char* sym)
{
unsigned int hash = 0;
unsigned c;
while(c=*sym++){hash = hash*9 ^ c;}

return hash;
}



struct symbol* lookup(char* sym)
{
struct symbol *sp = &symtab[symhash(sym)%NHASH];

int scount = NHASH;

while(--scount>=0)
{
if(sp->name && !strcmp(sp->name, sym))
{sp->freq++;
return sp;
}

if(!sp->name)
{
sp->name = strdup(sym);
sp->freq= 1;
sp->reflist = 0;
return sp;
}

if(++sp >= symtab+NHASH) 
sp = symtab;

}
fputs("Symbol table full, lookup failed!\n", stderr);
abort();

}



void addref(int lineno, char* filename, char* word, int flags)
{
struct ref *r, *head;
struct symbol *sp = lookup(word);

/*the reference has already been added*/
if(sp->reflist&&sp->reflist->lineno == lineno&&sp->reflist->filename==filename)
return;

r = malloc(sizeof(struct ref));
if(!r) 
{
fputs("Malloc in addref not working!\n", stderr);
abort();
}

/* adding before the existing reference */
r->filename = filename;
r->flags = flags;
r->lineno = lineno;

/* the head pointer is now changed to the newly added reference */
r->next = sp->reflist;
sp->reflist = r;

/*
head = sp->reflist;
while(head->next)
head = head->next;

head->next = r;
*/

} 

static int symcompare(const void* xa, const void* xb)
{
const struct symbol *a = xa;
const struct symbol *b = xb;

if(!a->name)
{
if(!b->name)
return 0;
else
return 1;
}
if(!b->name)
return -1;

return strcmp(a->name, b->name);
}


void printrefs()
{
struct symbol* sp;

qsort(symtab, NHASH, sizeof(struct symbol), symcompare);

for(sp = symtab; sp->name && sp<symtab+NHASH; sp++)
{
char* prevfn = NULL;

/* reverse the list of references */

struct ref *rp = sp->reflist;
struct ref *rpp = 0;
struct ref *rpn;

do
{
rpn = rp->next;
rp->next = rpp;
rpp = rp;
rp = rpn;
} while(rp);



printf("%s- freq: %d, ", sp->name, sp->freq);


for(rp = rpp;rp;rp = rp->next)
{
	if(rp->filename==prevfn)
	{
		printf(" %d", rp->lineno);
	}
	else
	{
		printf(" %s:%d", rp->filename, rp->lineno);
		prevfn = rp->filename;
	}
}
printf("\n");
}

}











