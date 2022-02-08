#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
typedef struct prod
{
char left;
char* right;
}*PPTR;

int num = 0;
char* getSymbol()
{
    if(num==0)
        {num++;return "@";}
    else if(num==1)
        {num++;return "#";}
    else
        return "&";
}

PPTR initprod(char* left, char* right)
{
PPTR N = malloc(1000);
N->right = malloc(100);
N->left = left[0];
strcpy(N->right, right);
return N;
}

//a function to check if a grammar symbol is a terminal
int isTerminal(char sym)
{
if(sym>='a'&&sym<='z')
return 1;
if(sym>='0'&&sym<='9')
return 1;
if(sym=='!')
return 1;

return 0; //otherwise
}

int power(int l)
{
    int k =1;
    int i=0;
    while(i<l)
    {
        k= k<<1;
        i++;
    }
    return k;
}

int isNullable(char sym, char* nullsym, int size)
{
	for(int i=0;i<size;i++)
	{
		if(sym==nullsym[i])
		return 1;
	}
	return 0;
}

int getNullable(char* nullsym, int size, PPTR p[], int s)
{
while(1)
{
	int oldsize = size;
	for(int i=0;i<s;i++)
	{
		int confirm = 0;
		for(int j=0;j<strlen(p[i]->right);j++)
		{
			char cs = p[i]->right[j];
			int found = 0;
			for(int k=0;k<size;k++)
			{
				if(nullsym[k]==cs)
				found = 1;
			}
			if(found)
			confirm++;
		}
		if(confirm==strlen(p[i]->right))
		{
			char cs = p[i]->left;
			int found = 0;
			for(int k=0;k<size;k++)
			{
				if(nullsym[k]==cs)
				found = 1;
			}
			if(!found)
			nullsym[size++] = p[i]->left;
		}
	}

	if(size==oldsize)
	break;
}
return size;
}

int getAllUnitProd(PPTR unit[], int us)
{
    while(1)
    {
        int oldus = us;
        for(int i=0;i<us;i++)
        {
            for(int j=0;j<us;j++)
            {
                if(unit[i]->right[0]==unit[j]->left&&unit[i]->left!=unit[j]->right[0])
                {
                    char* left = malloc(2);
                    left[0] = unit[i]->left;
                    left[1] = '\0';
                    PPTR newprod = initprod(left, unit[j]->right);
                    int found = 0;
                    for(int k=0;k<us;k++)
                    {
                        if(unit[k]->left==newprod->left && strcmp(unit[k]->right, newprod->right)==0)
                        {
                            found = 1;
                            break;
                        }
                    }
                    if(!found)
                    {
                        unit[us++] = newprod;
                    }
                }
            }
        }
        if(oldus==us)
            break;
    }
    return us;
}

int getGenerating(char* gensym, int size, PPTR p[], int s)
{

while(1)
{
	int oldsize = size;
	for(int i=0;i<s;i++)
	{
		int confirm = 0;
		for(int j=0;j<strlen(p[i]->right);j++)
		{
			char cs = p[i]->right[j];
			int found = 0;
			for(int k=0;k<size;k++)
			{
				if(gensym[k]==cs)
				found = 1;
			}
			if(found)
			confirm++;
		}
		if(confirm==strlen(p[i]->right))
		{
			char cs = p[i]->left;
			int found = 0;
			for(int k=0;k<size;k++)
			{
				if(gensym[k]==cs)
				found = 1;
			}
			if(!found)
			gensym[size++] = p[i]->left;
		}
	}
	if(oldsize == size)
	break;
}

return size;

}

int getReachable(char* reachsym, int size, PPTR p[], int s)
{

for(int i=0;i<size;i++)
{
	char rs = reachsym[i];
	for(int j=0;j<s;j++)
	{
		if(rs==p[j]->left)
		{
			//add every symbol in p[j]->right if it isn't already there.
			for(int k=0;k<strlen(p[j]->right);k++)
			{
				char cs = p[j]->right[k];
				int found = 0;
				for(int l=0;l<size;l++)
				{
					if(cs==reachsym[l])
					found =1;
				}
				if(!found)
				reachsym[size++] = cs;
			}
		}
	}
}

return size;

}

char* getNewProd(char* locnull, int locs, int* indices, int inds, int a, char* right)
{
	char* newprod = malloc(100);
	int news =0;
	int i=0;
	for(int j=0;j<strlen(right);j++)
	{
		if(isTerminal(right[j]))
			newprod[j] = right[j];
		else
			newprod[j] = '#';
	}
	newprod[strlen(right)] = '\0';

	while(a!=0)
	{
		if(a&1)
		{
			newprod[indices[i]] = locnull[i];
		}
		a=a>>1;
		i++;
	}

	char* finalprod = malloc(100);
	int f_ind=0;
	for(int j=0;j<strlen(newprod);j++)
	{
		if(newprod[j]!='#')
		finalprod[f_ind++] = newprod[j];
	}

	finalprod[f_ind] = '\0';
	return finalprod;

}

int main()
{

PPTR p[100];
int s=0;

char* left = malloc(100);
char* right = malloc(100);

while(1)
{

scanf("%s", left);

if(strcmp(left, "0")==0)
break;

scanf("%s", right);

p[s++] = initprod(left, right);
}


//assuming ! means null

char nullsym[100];
int size = 0;

PPTR nul[100];
int nulls = 0;

for(int i=0;i<s;i++)
{
	if(strcmp(p[i]->right, "!")!=0)
	nul[nulls++] = p[i];
	else
	nullsym[size++] = p[i]->left;
}

s = nulls;
for(int i=0;i<s;i++)
p[i] = nul[i];

//remove productions of the form A->! and add A to nullable symbols, base case.

size = getNullable(nullsym, size, p, s);

printf("Nullable symbols: \n");
for(int i=0;i<size;i++)
printf("%c ", nullsym[i]);
printf("\n");

//eliminating the null productions
nulls =0;
for(int i=0;i<s;i++)
{
	nul[nulls++] = p[i];
	char locnull[100];
    int locs =0;
    int indices[100];
    int inds =0;
	for(int j=0;j<strlen(p[i]->right);j++)
	{

		if(isNullable(p[i]->right[j], nullsym, size))
		{
			locnull[locs++] = p[i]->right[j];
			indices[inds++] = j;
		}
	}
	for(int j=0;j<power(locs)-1;j++)
	{
		char* newprod = getNewProd(locnull, locs, indices, inds, j, p[i]->right);
		char* left = malloc(2);
		left[0] = p[i]->left;
		left[1] = '\0';
		if(strlen(newprod)!=0)
		nul[nulls++] = initprod(left, newprod);
	}
}
s = nulls;
for(int i=0;i<s;i++)
p[i] = nul[i];

printf("Grammar without null productions: \n");
for(int i=0;i<s;i++)
printf("%c %s\n", p[i]->left, p[i]->right);

//all null productions have been eliminated.

//eliminating unit productions

PPTR nounit[100];
int nous = 0;

PPTR unit[100];
int us = 0;

for(int i=0;i<s;i++)
{
    if(strlen(p[i]->right)!=1 || isTerminal(p[i]->right[0]))
    {
        nounit[nous++] = p[i];
    }
    else
        unit[us++] = p[i];
}

us = getAllUnitProd(unit, us);

for(int i=0;i<us;i++)
{
    for(int j=0;j<nous;j++)
    {
        if(unit[i]->right[0]==nounit[j]->left)
        {
            char* left = malloc(2);
            left[0] = unit[i]->left;
            left[1] = '\0';
            PPTR newprod = initprod(left, nounit[j]->right);
            int found = 0;
            for(int k=0;k<nous;k++)
            {
                if(nounit[k]->left==newprod->left && strcmp(nounit[k]->right, newprod->right)==0)
                {
                    found = 1;
                    break;
                }
            }
            if(!found)
            {
                nounit[nous++] = newprod;
            }
        }
    }
}
printf("Grammar without unit productions: \n");
for(int i=0;i<nous;i++)
printf("%c %s\n", nounit[i]->left, nounit[i]->right);

s = nous;
for(int i=0;i<nous;i++)
p[i] = nounit[i];

//all unit productions have been eliminated.


//getting all generating symbols

char gensym[100];
size = 0;

char term[100];
int tsize = 0;


for(int i=0;i<s;i++)
{
	for(int j=0;j<strlen(p[i]->right);j++)
	{
		char cs = p[i]->right[j]; //current symbol
		if(isTerminal(cs))
		{
			term[tsize++] = cs;
			int found = 0;
			for(int k=0;k<size;k++)
			{
				if(gensym[k]==cs)
				found = 1;
			}
			if(!found)
			gensym[size++] = cs;
		}
	}
}

size = getGenerating(gensym, size, p, s);

printf("Generating symbols: \n");
for(int i=0;i<size;i++)
printf("%c ", gensym[i]);
printf("\n");

//eliminating non generating symbols.
PPTR gen[s];
int gens = 0;
for(int i=0;i<s;i++)
{
	char cs = p[i]->left;
	int found = 0;
	for(int k=0;k<size;k++)
	{
		if(gensym[k]==cs)
		found = 1;
	}
	if(!found)
	goto finish; //don't add any non generating symbol productions.

	for(int j=0;j<strlen(p[i]->right);j++)
	{
		char cs = p[i]->right[j]; //current symbol.
		int found = 0;
		for(int k=0;k<size;k++)
		{
			if(gensym[k]==cs)
			found = 1;
		}
		if(!found)
		goto finish; //don't add this production if it has a non generating symbol.
	}
	gen[gens++] = p[i];
	finish:;
}

for(int i=0;i<gens;i++)
printf("%c %s\n", gen[i]->left, gen[i]->right);

s = gens;
for(int i=0;i<s;i++)
p[i] = gen[i];

char reachsym[100];
size = 0;

reachsym[size++] = p[0]->left; //the start state is always reachable.

size = getReachable(reachsym, size, p, s);

printf("Reachable symbols: \n");
for(int i=0;i<size;i++)
printf("%c ", reachsym[i]);
printf("\n");

//eliminating non reachable symbols.
PPTR rec[s];
int recs = 0;
for(int i=0;i<s;i++)
{
	char cs = p[i]->left;
	int found = 0;
	for(int k=0;k<size;k++)
	{
		if(reachsym[k]==cs)
		found = 1;
	}
	if(!found)
	goto finish2; //don't add any non generating symbol productions.

	for(int j=0;j<strlen(p[i]->right);j++)
	{
		char cs = p[i]->right[j]; //current symbol.
		int found = 0;
		for(int k=0;k<size;k++)
		{
			if(reachsym[k]==cs)
			found = 1;
		}
		if(!found)
		goto finish2; //don't add this production if it has a non generating symbol.
	}
	rec[recs++] = p[i];
	finish2:;
}
for(int i=0;i<recs;i++)
printf("%c %s\n", rec[i]->left, rec[i]->right);

s = recs;
for(int i=0;i<s;i++)
p[i] = rec[i];

//now we have a grammar without useless symbols.

//getting rid of left recursion
char var[100];
int vsize = 0;

for(int i=0;i<s;i++)
{
	for(int j=0;j<strlen(p[i]->right);j++)
	{
		char cs = p[i]->right[j]; //current symbol
		if(!isTerminal(cs))
		{
		    int found = 0;
		    for(int k=0;k<vsize;k++)
		    {
			if(cs==var[k])
			{
			     found = 1;
			     break;
			}
		    }
   		if(!found)
		var[vsize++] = cs;
		}
	}
}


for(int i=0;i<vsize;i++)
{
    for(int j=0;j<i;j++)
    {
        for(int k=0;k<s;k++)
        {
            if(p[k]->left==var[i]&&p[k]->right[0]==var[j])
            {
                char* gamma = malloc(100);
                for(int l = 0;l<strlen(p[k]->right)-1;l++)
                {
                    gamma[l] = p[k]->right[l+1];
                }
                gamma[strlen(p[k]->right)-1] = '\0';

                char* delta = malloc(100);
                int flag = 0;
                for(int l=0;l<s;l++)
                {
                    if(p[l]->left == var[j])
                    {
                        strcpy(delta, p[l]->right);
                        strcat(delta, gamma);
                        char* left = malloc(2);
                        left[0] = var[i];
                        left[1] = '\0';
                        PPTR newprod = initprod(left, delta);
                        int found = 0;
                        for(int m=0;m<s;m++)
                        {
                            if(p[m]->left==newprod->left && strcmp(p[m]->right, newprod->right)==0)
                            {
                                found = 1;
                                break;
                            }
                        }
                        if(!found)
                        {
                            if(!flag)
                            {
                             flag=1;
                             p[k] = newprod;
                             //the first production, just replace
                            }
                            else
                            p[s++] = newprod; //remaining, add
                        }

                    }
                }
                //removing the Ai -> Aj gamma production from the grammar.
            }
        }
    }

    //removing immediate left recursion among Ai productions
    char* Adash = malloc(100);
    strcpy(Adash, getSymbol());

    for(int j=0;j<s;j++)
    {
        if(p[j]->left==var[i])
        {
            if(p[j]->right[0] ==var[i])
            {
                char* alpha = malloc(100);
                for(int k=0;k<strlen(p[j]->right)-1;k++)
                {
                    alpha[k] = p[j]->right[k+1];
                }
                alpha[strlen(p[j]->right)-1] = '\0';
                strcat(alpha, Adash);

                p[j] = initprod(Adash, alpha); //just replace
            }
            else
            {
                char* right = malloc(100);
                strcpy(right, p[j]->right);
                strcat(right, Adash);

                char* left = malloc(2);
                left[0] = p[j]->left;
                left[1] = '\0';
                p[j] = initprod(left, right);//just replace
            }
        }
    }

    p[s++] = initprod(Adash, "!");
}

printf("Grammar without left recursion:\n");
for(int i=0;i<s;i++)
printf("%c %s\n", p[i]->left, p[i]->right);


return 0;
}


/*
S AB
S C
A 0B
A C
B 1
B A0
C AC
C C1
0
//grammar for removing useless symbols


S ABCd
A BC
B bB
B !
C cC
C !
0
//grammar for removing nullable symbols

S Aa
S B
A b
A B
B a
B A
0

//grammar for removing unit productions

S SX
S SSb
S XS
S a
X Xb
X Sa
X b
0
//grammar for removing left recursion

S SB
S C
A CB
A B
A D
C a
C !
B b 
B !
R !
0

*/
