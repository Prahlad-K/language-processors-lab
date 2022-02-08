#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
typedef struct prod
{
char left;
char* right;
}*PPTR;

typedef struct ref
{
    char* var;
    struct entry* where[100];
}*RPTR;

typedef struct entry
{
    char* lvar;
    RPTR right[100];
}*EPTR;

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


RPTR initref(char* var, EPTR where[])
{
    RPTR R = malloc(100);
    R->var = malloc(100);
    strcpy(R->var, var);

    if(where==NULL)
    {
        for(int i=0;i<strlen(var);i++)
        {
            R->where[i] = NULL;
        }
    }
    else
    {
        for(int i=0;i<strlen(var);i++)
        {
            R->where[i] = where[i];
        }
    }
    return R;
}

EPTR initentry(char* lvar, RPTR right[])
{
    EPTR E = malloc(100);
    E->lvar = malloc(100);
    strcpy(E->lvar, lvar);
    for(int i=0;i<strlen(lvar);i++)
    {
        E->right[i] = right[i];
    }
    return E;
}

PPTR initprod(char* left, char* right)
{
PPTR N = malloc(1000);
N->right = malloc(100);
N->left = left[0];
strcpy(N->right, right);
return N;
}

int getCombinations(char* X1, char* X2, char** comb, int* is, int *js)
{
    int i=0;
    char* newcomb = malloc(100);
    for(int j=0;j<strlen(X1);j++)
    {
        for(int k=1;k<strlen(X2);k++)
        {
            if(j<k)
            {
                memset(newcomb, '\0', 100);

                newcomb[0] = X1[j];
                newcomb[1] = X2[k];
                newcomb[2] = '\0';

                comb[i] = malloc(100);
                strcpy(comb[i], newcomb);

                i++;
            }
        }
    }
    return i;

}


void derivate(EPTR current, char symbol)
{
	//printf("Inside %c\n", symbol);
	//if(!isTerminal(symbol))
	//{
		for(int i=0;i<strlen(current->lvar);i++)
		{
			if(symbol==current->lvar[i])
			{
				if(current->right[i]==NULL)
				{
				return;
				}
				else
				{
					int x = strlen(current->right[i]->var);
					//printf("%d x\n", x);
				if(x==1)
					printf("%c->%c\n", symbol, current->right[i]->var[0]);
				if(x==2)
				{
					printf("%c->%s\n", symbol, current->right[i]->var);
					for(int j=0;j<strlen(current->right[i]->var);j++)			
						derivate(current->right[i]->where[j], current->right[i]->var[j]);
				}
				else
				{
					for(int k=0;k<x/2;k++)
					{
						char* exp = malloc(3);
						exp[0] = current->right[i]->var[k*2];
						exp[1] = current->right[i]->var[k*2+1];
						exp[2] ='\0';
						
						printf("%c->%s\n", symbol, exp);
						for(int j=k*2;j<k*2+2;j++)
							derivate(current->right[i]->where[j], current->right[i]->var[j]);
						printf("Another possible derivation sequence\n");
					}
				}
				}
			}
		}
	//}
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

char* w = malloc(100);
scanf("%s", w);

int l = strlen(w);

EPTR E[100][100];

for(int i=1;i<=l;i++)
{
    char* lvar = malloc(100);
    memset(lvar, '\0', 100);

    char* var = malloc(2);
    memset(var, '\0', 2);

    var[0] = w[i-1];
    var[1] = '\0';

    for(int j=0;j<s;j++)
    {
        if(strlen(p[j]->right)==1 && p[j]->right[0] ==w[i-1])
        {
            char* left = malloc(2);
            left[0] = p[j]->left;
            left[1] = '\0';

            strcat(lvar, left);
        }
    }

    RPTR right[100];
    for(int j=0;j<strlen(lvar);j++)
        right[j] = initref(var, NULL);

    E[i][i] = initentry(lvar, right);
    //attach all possible variables.
}
char* lvar = malloc(100);
for(int diff = 1;diff <= l-1; diff++)
{
    for(int i=1;i<=l-diff;i++)
    {
        int j = i + diff;
        memset(lvar, '\0', 100);
        int lind =0;
        RPTR right[100];
        for(int k=i;k<=j-1;k++)
        {
            char* X1 = E[i][k]->lvar;
            char* X2 = E[k+1][j]->lvar;
            EPTR where[2];
            where[0] = E[i][k];
            where[1] = E[k+1][j];

            int m=0;
            char* newcomb = malloc(100);
            for(int v=0;v<strlen(X1);v++)
            {
                for(int q=0;q<strlen(X2);q++)
                {
                    if(1)
                    {
                        memset(newcomb, '\0', 100);

                        newcomb[0] = X1[v];
                        newcomb[1] = X2[q];
                        newcomb[2] = '\0';
                        //printf("newcomb %s\n", newcomb);
                        for(int r=0;r<s;r++)
                        {
                            if(strcmp(p[r]->right, newcomb)==0)
                            {
                                int found = 0, index;
                                //printf("%s comb found\n", newcomb);
                                for(int t=0;t<strlen(lvar);t++)
                                {
                                    if(lvar[t]==p[r]->left)
                                    {
                                        index = t;
                                        found = 1;
                                        //printf("%c found\n", lvar[t]);
                                    }
                                }
                                if(!found)
                                {
                                    lvar[lind] = p[r]->left;
                                    right[lind] = initref(newcomb, where);
                                    lind++;
                                }
                                else
                                {
                                    right[index]->where[strlen(right[index]->var)] = where[0];
                                    right[index]->where[strlen(right[index]->var)+1] = where[1];
                                    strcat(right[index]->var, newcomb);
                                }
                            }
                        }
                    }
                }
            }
        }
        //now I have got lvar and right

        E[i][j] = initentry(lvar, right);
        printf("i= %d, j = %d, lvar = %s\n", i, j, lvar);
    }
}

int count =1;
for(int i=1;i<=l;i++)
{
    for(int j=i;j<=l;j++)
    {
        printf("%s ", E[i][j]->lvar);
/*
        for(int k=0;k<=strlen(E[i][j]->lvar);k++)
        {
            if(E[i][j]->right[k])
            {int x = strlen(E[i][j]->right[k]->var)/2;
                count = count* x;
            }
        }
*/
    }
    printf("\n");
}

for(int i=0;i<strlen(E[1][l]->lvar);i++)
{
    if(E[1][l]->lvar[i]=='S')
    {
        printf("The given string is a member of the given grammar.\n");
	derivate(E[1][l], 'S');
	return 0;
    }

}
printf("The given string is not a member of the given grammar.\n");


return 0;
}
/*
S AB
S BC
A BA
A a
B b
B CC
C AB
C a
0
baaba
*/
