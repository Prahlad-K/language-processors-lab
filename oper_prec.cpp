
#include <bits/stdc++.h> 
#include <string>


using namespace std;



typedef struct prod
{
	char left;
	string right;
}*PPTR;

PPTR prods[100];

string terminals[100];
int nt = 4;

int nop=0;
int addprod(char left, string right)
{
   	PPTR p1 = new prod();
	p1->left = left;
	p1->right = right;
	prods[nop++] = p1;
	return 1;
}

int index(string t)
{
	for(int i=0;i<4;i++)
	{
		if(t==terminals[i])
		return i;
	}
	cout<<"Invalid terminal!\n";
	return -1;
}

void reduce_print(string handle, vector <string> s, vector<string> input)
{
	
	if(handle=="+"||handle=="*")
	{	
		handle = handle + "E";
		handle = "E" + handle;
	
		//cout<<"Reducing "<<handle<<" to E"<<endl;

		
	}
	//cout<<"E -> "<<handle<<endl;		
	string sent_form = "";

	//cout<<"Contents of stack:\n";
	for(vector<string>::iterator it = s.begin();it!=s.end();it++)
	{
		//cout<<*it<<endl;
		sent_form += *it;	
	}
	//cout<<"Contents of input:\n";
	for(int i=0;i<input.size();i++)
	{
		//cout<<input[i]<<endl;
		sent_form +=input[i];
	}
	cout<<sent_form<<endl;
}


bool complete(vector<string> s, vector<string> tokens, int first, int l)
{
	string sent_form;
	//cout<<"Contents of stack:\n";
	for(vector<string>::iterator it = s.begin();it!=s.end();it++)
	{
		//cout<<*it<<endl;
		sent_form += *it;	
	}
	//cout<<"Contents of input:\n";
	for(int i=0;i<tokens.size();i++)
	{
		//cout<<input[i]<<endl;
		sent_form +=tokens[i];
	}
	if(sent_form!="$E$")
	return false;
	else
	{
		cout<<sent_form<<endl;
		return true;
	}
}

int main()
{

	terminals[0] = "id";
	terminals[1] = "+";
	terminals[2] = "*";
	terminals[3] = "$";

	addprod('E',"id");
	addprod('E', "E+E");
	addprod('E', "E*E");

	char prectab[nt][nt];
	string inp;
	cout<<"Enter input:";
	
	getline(cin, inp);

	cout<<"Enter the precedence table:\n";

	for(int i=0;i<nt;i++)
	{
		for(int j=0;j<nt;j++)
		{
			cin>>prectab[i][j];
		}
	}
	cout<<"Precedence table is given as follows:\n";

	for(int i=0;i<nt;i++)
	{
		for(int j=0;j<nt;j++)
		{
			cout<<prectab[i][j]<<" ";
		}
		cout<<endl;
	}
	vector <string> tokens;
	stringstream check1(inp); 
	string intermediate;
	while(getline(check1, intermediate, ' ')) 
    	{ 
	 //cout<<intermediate<<endl;
       	 tokens.push_back(intermediate); 
   	} 
			
	vector<string> s;
	s.push_back("$");
	
	vector<string>:: iterator it = tokens.begin();

	cout<<"The sentential forms of the grammar are done as follows:\n";
	
	int first = 1;
	int l = s.size();
	while(!complete(s, tokens, first, l))
	{
		if(first)
			first = 0;
		
		if(s[l-1]=="E")
		{
			
			l--;
			//cout<<l<<" l value\n";
			continue;
		}
		
		if(prectab[index(s[l-1])][index(*it)]=='<')
		{
			//top of the stack has lower precedence
			//cout<<"Shifting!\n";
			s.push_back(*it);
			tokens.erase(it);
			it = tokens.begin();
			l = s.size();
			//shift onto the top of the stack
		}
		else if(prectab[index(s[l-1])][index(*it)]=='>')
		{
			reduce_print(s[l-1], s, tokens);
			if(s[l-1]=="+"||s[l-1]=="*")
			{
				s.pop_back();
				s.pop_back();
				s.pop_back();
			}
			else
				s.pop_back();
			s.push_back("E");
		}	
		else
		{
			cout<<s[l-1]<<" "<<*it<<endl;
			cout<<"input error!\n";
			return 1;
		}
	}
	
	return 0;
}




