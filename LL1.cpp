#include <bits/stdc++.h>
#include <string>
using namespace std;

char start_symbol;
struct rule
{
    char left;
    string right;
};

bool isTerminal(char a, struct rule r[], int n)
{
    //if it's none of the non terminals, it's a terminal!
    for(int i=0;i<n;i++)
    {
            if(r[i].left==a)
                return false;
    }
    return true;
}


bool isNullable(char c, struct rule r[], int n)
{
    if(c=='!')
        return true;

    if(isTerminal(c, r, n))
        return false;

    for(int i=0;i<n;i++)
    {
        if(r[i].left==c)
        {
            for(int j=0;j<r[i].right.length();j++)
            {
                if(isNullable(r[i].right[j], r, n))
                    return true;
            }
        }
    }
    return false;
}

string addset(string s1, string s2)
{
    for(int i=0;i<s2.length();i++)
    {
        if(s1.find(s2.substr(i, 1))==string::npos)
        {
            s1 = s1 + s2.substr(i, 1);
        }
    }
    return s1;
}

set <char> first(string alpha, map<string, set<char> > &fir, struct rule r[], int n)
{
    //calculating the first of alpha
	if(fir.find(alpha)!=fir.end())
	{
		return fir[alpha];
	}
	else
	{

		if(alpha.size()==1)
		{
			if(isTerminal(alpha[0], r, n))
			{
				//cout<<alpha[0]<<endl;
				fir[alpha].insert(alpha[0]);
				return fir[alpha];
			}
			else
			{
				for(int i=0;i<n;i++)
				{
					if(alpha[0]==r[i].left)
					{
						for(int j=0;j<r[i].right.size();j++)
						{
							set<char> myset = first(r[i].right.substr(j, 1), fir, r, n);
							if(myset.find('!')==myset.end())
							{
								// ! not found, end
								fir[alpha].insert(myset.begin(), myset.end());
								break;
							}
							else
							{
								// ! found, means will have to explore next ones also
								if(j!=r[i].right.size()-1)
									myset.erase('!');
								fir[alpha].insert(myset.begin(), myset.end());
							}
						}
					}
				}
				return fir[alpha];

			}
		}
		else
		{
			//if there are many symbols in alpha
			for(int j=0;j<alpha.size();j++)
			{
				set<char> myset = first(alpha.substr(j, 1), fir, r, n);
				if(myset.find('!')==myset.end())
				{
					// ! not found, end
					fir[alpha].insert(myset.begin(), myset.end());
					break;
				}
				else
				{
					// ! found, means will have to explore next ones also
					if(j!=alpha.size()-1)
						myset.erase('!');
					fir[alpha].insert(myset.begin(), myset.end());
				}
			}
			return fir[alpha];
		}
	}
}

set<char> follow(char alpha,  map<char, set<char> > &fol, map<string, set<char> > &fir, struct rule r[], int n)
{
	if(fol.find(alpha)!=fol.end())
	{
		return fol[alpha];
	}
	else
	{
		if(isTerminal(alpha, r, n))
		{
			cout<<"Invalid request for follow!";
		}
		else
		{
			if(alpha==start_symbol)
			{
				fol[alpha].insert('$');
			}
			for(int i=0;i<n;i++)
			{
				if(r[i].right.find(alpha)!=string::npos)
				{

					int pos = r[i].right.find(alpha);
					string beta;

					if(pos==r[i].right.length()-1)
					{//found a production of the form X -> gamma 'alpha'
						set<char> follow_of_X = follow(r[i].left, fol, fir, r, n);
						fol[alpha].insert(follow_of_X.begin(), follow_of_X.end());
					}
					else
					{//found a production of the form X -> gamma 'alpha' beta

						beta = r[i].right.substr(pos+1); //getting the beta

						set<char> first_of_beta = first(beta, fir, r, n); //getting the first of beta

						fol[alpha].insert(first_of_beta.begin(), first_of_beta.end());
						set<char>::iterator nullpos = fol[alpha].find('!');
						if(nullpos!=fol[alpha].end())
						{
							fol[alpha].erase(nullpos);
							//not adding nulls to follow.
							//if ! is there in first(beta) then follow(X) must be added to follow(alpha)
							set<char> follow_of_X = follow(r[i].left, fol, fir, r, n);
							fol[alpha].insert(follow_of_X.begin(), follow_of_X.end());
						}

					}

				}
			}
			return fol[alpha];
		}
	}
}


int main()
{

	int n;
	cout<<"Enter number of rules:\n";
	cin>>n;
	struct rule r[n];
	vector<char> nonTerminals;

	for(int i=0;i<n;i++)
	{
		cin>>r[i].left>>r[i].right;
	    if(i==0)
            start_symbol = r[i].left;
	}
    map<char, set<char> > fol;
    map<string, set<char> > fir;
	/*
	testing of first and follow
	cin>>alpha;

	set<char> res = first(alpha, fir, r, n);
	for(set<char>::iterator it = res.begin();it!=res.end();it++)
	{
	cout<<*it<<" ";
	}
	cout<<endl;

    char alpha;
	cin>>alpha;
	//map<string, set<char> > fir;
	//map<char, set<char> > fol;
	set<char> res = follow(alpha, fol, fir, r, n);
	for(set<char>::iterator it = res.begin();it!=res.end();it++)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
*/



	//the construction of the predictive parsing table

	string w;
	cout<<"Enter input string: ";
	cin>>w;
	set<char> input_symbols;

	for(int i=0;i<w.length();i++)
	{
		input_symbols.insert(w[i]);
	}

	map<pair<char, char>, struct rule> M;

	for(vector<char>:: iterator it = nonTerminals.begin();it!=nonTerminals.end();it++)
	{
		for(set<char>:: iterator jt = input_symbols.begin();jt!=input_symbols.end();jt++)
		{
			pair<char, char> index;
			struct rule prod;
			prod.left = '#'; //indicates empty
			prod.right = "#";
			index = make_pair(*it, *jt);
			M[index] = prod;
		}
	}

	//The parsing table is suucessfully initialized!

	cout<<"The parsing table entries are as follows:\n";
	for(int i=0;i<n;i++)
	{
		//consider a production of the type
		set<char> first_of_alpha = first(r[i].right, fir, r, n);
		for(set<char>:: iterator it = first_of_alpha.begin();it!=first_of_alpha.end();it++)
		{
			char a = *it;
			if(a!='!')
			{
				M[make_pair(r[i].left, a)] = r[i]; //step 1 of the algorithm
                cout<<r[i].left<<", "<<a<<": "<<r[i].left<<"->"<<r[i].right<<endl;
			}
			else
			{
				set<char> follow_of_A= follow(r[i].left, fol, fir, r, n);
				for(set<char>:: iterator jt = follow_of_A.begin(); jt!=follow_of_A.end(); jt++)
				{
					char b = *jt;
					M[make_pair(r[i].left, b)] = r[i]; //step 2, part 1 of the algorithm
					cout<<r[i].left<<", "<<b<<": "<<r[i].left<<"->"<<r[i].right<<endl;
					if(b=='!')
					{
						M[make_pair(r[i].left, '$')] = r[i];
						cout<<r[i].left<<", "<<'$'<<": "<<r[i].left<<"->"<<r[i].right<<endl;
					}
				}
			}
		}
	}

	// and our predictive parser is ready!

	cout<<"The parser is being evaluated....\n";
	cout<<"Here are the results:\n";
	int ip = 0;
	char a = w[ip];
	stack <char> s;
	s.push('$');
	s.push(start_symbol);
	char X = s.top();

	while(X!='$')
	{
		if(X==a)
		{
			s.pop();
			ip++;
			a = w[ip];
		}
		else if(isTerminal(X, r, n))
		{
		    cout<<"X is "<<X<<endl;
			cout<<"Error! X is a terminal\n";
			break;
		}
		else if(M[make_pair(X, a)].left=='#')//meaning empty/error
		{
		    cout<<"X and a are "<<X<<", "<<a<<endl;
			cout<<"Error, no entry exists in the table!\n";
			break;
		}
		else if(M[make_pair(X, a)].left!='#')
		{
			//assuming production is of form X -> Y1 Y2 Y3 .... Yk
			cout<<X<<"->"<<M[make_pair(X, a)].right<<endl;
			s.pop();
			for(int i=M[make_pair(X,a)].right.length()-1;i>=0;i--)
			{
			    if(M[make_pair(X,a)].right[i]!='!') //only insert new ones onto the stack if not null
				s.push(M[make_pair(X,a)].right[i]); //pushing Yk Yk-1 and so on upto Y1 onto the stack.
			}
		}
		X= s.top();
	}
	//and lo, parsing done!

}


/*
FIRST and FOLLOW example:
9
S ACB
S Cbb
S Ba
A da
A BC
B g
B !
C h
C !

FIRST(S) = {d, g, h, !, b, a}
FIRST(A) = {d, g, h, !}
FIRST(B) = {g, !}
FIRST(C) = {h, !}

FOLLOW(S) = {$}
FOLLOW(A) = {h, g, $}
FOLLOW(B) = {a, $, h, g}
FOLLOW(C) = {b, g, $, h}

PREDICTIVE PARSER INPUT:
8
E TA
A +TA
A !
T FB
B *FB
B !
F (E)
F i
i+i*i$

Note: A indicates E'
      B indicates T'
*/
