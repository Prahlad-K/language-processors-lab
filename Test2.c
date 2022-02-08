#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
	
	printf("Num %s\n", num);
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


int main(int argc, char* argv[])
{

	char* res = malloc(100);

	res = swap(argv[1]);

	printf("%s\n", res);

}
