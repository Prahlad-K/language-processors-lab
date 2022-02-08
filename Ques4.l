/*  Program to encrypt the text using lex */

%{

int k;
char* decrypText;

char* cycle(char*);
char* addAscii(char*, int k);
char* swap(char*);

%}

%%

sp1 |
sp2 |
sp3 {strcat(decrypText, cycle(yytext));}

[a-zA-Z]+ {strcat(decrypText, addAscii(yytext, k));} 

[0-9]+ {strcat(decrypText, swap(yytext));}

[\n\t ]+ {strcat(decrypText, " ");}

%%

//function to perform 1)
char* cycle(char* s)
{

	char* news = malloc(100);

	char* headnews;
	headnews = news;

	char* heads;
	heads = s;
	s = s+2;

	while(*s!='\0')
	{
	*news = *s;
	news++;
	s++;
	}

	s--;

	//copying the entire string except the first two.

	s = heads;
	*news = *s;
	news++;
	s++;
	*news = *s;

	//adding the first two at the back - rotating right by 2 complete.
	char* reverse = malloc(100);
	reverse = news; 
	char* headres;
	headres = reverse;
	while(news!=headnews)
	{
	*reverse = *news;
	reverse++;
	news--;
	}
	
	*reverse = *news;
	reverse = headres;
	return reverse;
	//reversing complete.
}


char* addAscii(char* s, int k)
{

	char* news = malloc(100);

	char* head;
	head= news;

	while(*s!='\0')
	{
	if((*s+k>='a'&&*s+k<='z')||(*s+k>='A'&&*s+k<='Z'))
	*news = (*s + k);
	else if(*s>='a'&&*s<='z')
	*news = (*s + k)%('z') + 'a' -1;
	else
	*news = (*s + k)%('Z') + 'A' -1;

	news++;
	s++;
	}
	
	char* end;
	news--;
	end = news;
	news = head;

	while(news<end)
	{
		char t = *news;
		*news = *end;
		*end = t;
		news++;
		end--;
	}
	news = head;
	return news;

}


char* swap(char* num)
{

	char* head;
	head = num;

	int l =0;
	while(*num!='\0')
	{
	l++;
	num++;
	}
	
	char* left = malloc(100);
	char* lstart;
	lstart = left;

	char* right = malloc(100);
	char* rstart;
	rstart = right;

	char* middle = malloc(100);

	char* result = malloc(100);
	num = head;
	
	for(int i=0;i<l/2;i++)
	{
	*left = *num;
	left++;
	num++;
	}
	left = lstart;
	
	if(l%2!=0)
	{*middle = *num; num++;}
	
	for(int i=0;i<l/2;i++)
	{
	*right = *num;
	right++;
	num++;
	}
	right = rstart;

	strcat(result, right);
	strcat(result, middle);
	strcat(result, left);

	return result;
}




main(int argc, char** argv)
{

decrypText = malloc(100);

if(argc<=2)
{
k = atoi(argv[1]);
yylex();
printf("%s", decrypText);
}
else
{
for(int i=2;i<argc;i++)
{
FILE* f = fopen(argv[i], "r");
yyrestart(f);
k = atoi(argv[1]);
yylex();
printf("%s", decrypText);
}

}
return 0;
}


