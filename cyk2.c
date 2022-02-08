#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct lNode;
typedef struct lNode* lptr;
struct lNode 
{ 
  char data; 
  lptr next; 
};
struct node;
typedef struct node* btptr;
struct node
{
	char data;
	struct node* lc;
	struct node* rc;
};
void insertAfter(lptr prev_node, int new_data) 
{ 
    lptr new_node =(lptr) malloc(sizeof(struct lNode));
    new_node->data  = new_data; 
    new_node->next = prev_node->next;  
    prev_node->next = new_node; 
}
lptr head;
void appen(btptr* s, char c,char ee) {
        int e=0;
        while(s[e]->data!='#')
        	e++;
        s[e]->data=c;	
        btptr t=malloc(sizeof(btptr));
        t->data=ee;
        t->lc=NULL;
        t->rc=NULL;
        s[e]->lc=t;
}
int strle(btptr *s)
{
	int e=0;
        while(s[e]->data!='#')
        	e++;
        return e;
}
void appen1(btptr*** cy,btptr* s, char c,int a1,int a2,int a3,int a4,int a5,int a6) {
        int e=0;
        while(s[e]->data!='#')
        	e++;
        s[e]->data=c;	
        s[e]->lc=cy[a1][a2][a3];
        s[e]->rc=cy[a4][a5][a6];
}
int sameas(char* right,char c)
{
	if((strlen(right) == 1) && right[0]==c)
		return 1;
	return 0;	
}

int sameas1(char* right,char c1,char c2)
{
	if((strlen(right) == 2) && right[0]==c1 && right[1]==c2)
		return 1;
	return 0;	
}
void nowprint(lptr ode) 
{ 
  while (ode != NULL) 
  { 
     printf("%c", ode->data); 
     ode = ode->next; 
  } 
	printf("\n");
}
void preorder(btptr t)
{
	if(t!=NULL)
	{
		printf("%c",t->data);
		preorder(t->lc);
		preorder(t->rc);
	}
}
lptr func2(lptr head, char x) 
{ 
    lptr current = head;  
    while (current != NULL) 
    { 
        if (current->data == x) 
            return current; 
        current = current->next; 
    } 
}
void func(btptr t)
{
	//printf("hi1\n");
	if(t!=NULL)
	{
		//printf("hi2\n");
		if(t->lc==NULL)
		{
			
		}
		else
		{
			//printf("hi3\n");
			if(t->rc==NULL)
			{
				lptr fin=func2(head,t->data);
				fin->data=t->lc->data;
				//printf("List is: ");
				nowprint(head);
				//printf("\n");
			}
			else
			{
				lptr fin=func2(head,t->data);
				fin->data=t->lc->data;
				insertAfter(fin,t->rc->data);
				//printf("List is: ");
				nowprint(head);
				//printf("\n");
			}
		}
		func(t->lc);
		func(t->rc);
	}
}
int main()
{
	int n;
//	printf("Enter no of rules\n");
//	scanf("%d",&n);
	n=8;
	char* arr = malloc(100* sizeof(char));
	char* left = malloc(n* sizeof(char));
	char** right = malloc(n* sizeof(char*));
	for(int j=0;j<n;j++)
		right[j]= malloc(100* sizeof(char));
	
	left[0]='S';
	left[1]='S';
	left[2]='A';
	left[3]='A';
	left[4]='B';
	left[5]='B';
	left[6]='C';
	left[7]='C';
	right[0]="AB";
	right[1]="BC";
	right[2]="BA";
	right[3]="a";
	right[4]="CC";
	right[5]="b";
	right[6]="AB";
	right[7]="a";
//	printf("Enter String\n");
//	scanf("%s",arr);
	arr="babbba";	
	btptr*** cy = malloc((strlen(arr))* sizeof(btptr**));
	for(int j1=0;j1<strlen(arr);j1++)
	{
		cy[j1]=malloc((strlen(arr)-j1)*sizeof(btptr*));
		for(int j2=0;j2<(strlen(arr)-j1);j2++)
		{
				cy[j1][j2]=malloc(100* sizeof(btptr));
				for(int j3=0;j3<100;j3++)
				{
					cy[j1][j2][j3]=malloc(sizeof(btptr));
					cy[j1][j2][j3]->data='#';
					cy[j1][j2][j3]->lc=NULL;
					cy[j1][j2][j3]->rc=NULL;
				}
		}
	}
	
	for(int j=0;j<strlen(arr);j++)
	{
		for(int j1=0;j1<n;j1++)
		{
			if(sameas(right[j1],arr[j]))
				appen(cy[0][j],left[j1],arr[j]);
		}
	}
	for(int j1=1;j1<strlen(arr);j1++)
	{
		for(int j2=0;j2<(strlen(arr)-j1);j2++)
		{
			for(int k=0;k<j1;k++)
			{
				for(int v1=0;v1<strle(cy[k][j2]);v1++)
				{
					for(int v2=0;v2<strle(cy[j1-1-k][j2+1+k]);v2++)
					{
						for(int v3=0;v3<n;v3++)
						{
							if(sameas1(right[v3],cy[k][j2][v1]->data, cy[j1-1-k][j2+1+k][v2]->data))
								appen1(cy,cy[j1][j2],left[v3],k,j2,v1,j1-1-k,j2+1+k,v2);
						}
					}
				}
			}
		}
	}
	for(int j1=0;j1<strlen(arr);j1++)
	{
		for(int j2=0;j2<(strlen(arr)-j1);j2++)
		{
			for(int j3=0;j3<strle(cy[j1][j2]);j3++)
				if(cy[j1][j2][j3]->data!='#')
					printf("%c",cy[j1][j2][j3]->data);
			printf(" ");					
		}
		printf("\n");	
	}
	printf("\n");	
	head=(lptr) malloc(sizeof(struct lNode));
	for(int j=0;j<strle(cy[strlen(arr)-1][0]);j++)
	{
		if(cy[strlen(arr)-1][0][j]->data=='S')
		{
			head->data='S';
			head->next=NULL;
			nowprint(head);
			func(cy[strlen(arr)-1][0][j]);
		}
		printf("\n");
		head=NULL;
		head=(lptr) malloc(sizeof(struct lNode));	
	}
}
