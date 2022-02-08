#include <bits/stdc++.h>

using namespace std;

char start_symbol;
struct rule
{
    char left;
    string right;
    char lookahead; //change from LR(0)
};

struct action_entry
{
    char s_r_a; //shift? reduce? accept?
    int state_number;
    struct rule prod;
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

bool areSame(vector<struct rule> I1, vector<struct rule> I2)
{
    for(int i=0;i<I1.size();i++)
    {
        if(I1[i].left!=I2[i].left||(I1[i].right.compare(I2[i].right)!=0)||(I1[i].lookahead!=I2[i].lookahead))
        return false;
    }
    return true;
}

vector<struct rule> closure(vector<struct rule> I, struct rule r[], int n)
{
	vector<struct rule> result;
    map<string, set<char> > fir;
    if(I.size()==0)
        return result;
	for(vector<struct rule> :: iterator it = I.begin();it!=I.end();it++)
	{
		result.push_back(*it);
		//every item in I is also in closure(I)
	}

	int done = 0;
	set<pair<char, char> > B_lookahead;
	while(!done)
	{
		done = 1;
		//assume you're done. If you're not, you'll be updated
		for(vector<struct rule> :: iterator it = result.begin();it!=result.end();it++)
		{
			int dot_pos = it->right.find(".");
			if(dot_pos!=it->right.length()-1)
			{
				if(!isTerminal(it->right[dot_pos+1], r, n))
				{
					//found an item of the form [A->alpha.Bbeta, a]
					//so we'd have to add B->.gamma, b where b is in first('beta'a)
					char B = it->right[dot_pos+1];
                    //cout<<"For prod: "<<it->left<<"->"<<it->right<<", "<<it->lookahead<<endl;
					string beta = it->right.substr(dot_pos+2);
					string beta_a = beta;
					beta_a.push_back(it->lookahead);
					//cout<<"Beta a "<<beta_a<<endl;
					//of course, it->lookahead is a.
					if(B_lookahead.find(make_pair(B, it->lookahead))==B_lookahead.end())
                    {
                        //cout<<"Need to add "<<B<<endl;
                        B_lookahead.insert(make_pair(B, it->lookahead));
                        for(int i=0;i<n;i++)
                        {
                            if(r[i].left==B)
                            {
                                //found a production B->gamma
                                struct rule item;
                                item.left = B;
                                item.right = "." + r[i].right;

                                set<char> first_beta_a;
                                first_beta_a = first(beta_a, fir, r, n);
                                for(set<char> ::iterator jt = first_beta_a.begin();jt!=first_beta_a.end();jt++)
                                {
                                    item.lookahead = *jt; //b is found
                                    //generating B->.gamma
                                    //cout<<"Generated "<<B<<"->"<<item.right<<endl;
                                    for(vector<struct rule>:: iterator vt = result.begin();vt!=result.end();vt++)
                                    {
                                        if(vt->left==item.left&&vt->right.compare(item.right)==0&&vt->lookahead==item.lookahead)
                                            goto dontadd;
                                    }

                                   // cout<<"Added item "<<item.left<<"->"<<item.right<<", "<<item.lookahead<<endl;
                                    result.push_back(item);
                                    it = result.begin();

                                    dontadd:
                                    done = 0;
                                    //not done yet!
                                }
                            }
                        }
                    }
				}
			}
		}
	}
	return result;
}

void print_item(vector<struct rule> I)
{
	for(vector<struct rule> :: iterator it = I.begin();it!=I.end();it++)
	{
		cout<<it->left<<"->"<<it->right<<", "<<it->lookahead<<endl;
	}
}

vector <struct rule> goto_function(vector <struct rule> I, char X, struct rule r[], int n)
{
    //X is a grammar symbol

    vector<struct rule> result;

    for(vector <struct rule>::iterator it = I.begin();it!=I.end();it++)
    {
        int dot_pos = it->right.find(".");
        if(dot_pos!=it->right.length()-1)
        {
            if(it->right[dot_pos+1]==X)
            {
                string new_right = it->right;
                new_right.erase(dot_pos, 1);
                new_right.insert(dot_pos+1, ".");

                struct rule new_item;
                new_item.left = it->left;
                new_item.right = new_right;
                new_item.lookahead = it->lookahead;

                result.push_back(new_item);
            }
        }
    }
    return closure(result, r, n);
}


bool there(vector<vector<struct rule> > canonical_LR_items, vector<struct rule> I)
{
    for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
    {
            if(areSame(*it, I))
            {
                return true;
            }
    }
    return false;
}


vector<vector<struct rule> > canonical_LR_items;

int item_to_number(vector<struct rule> I)
{
    int item_number = 0;
    for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
    {
        if(areSame(*it, I))
            return item_number;
        item_number++;
    }
    return -1; //not a member of the canonical set of items
}

vector<struct rule> get_unique(vector<struct rule> I)
{
    vector<struct rule> uniq;

    for(vector<struct rule> ::iterator it = I.begin();it!=I.end();it++)
    {
        for(vector<struct rule>::iterator jt = uniq.begin();jt!=uniq.end();jt++)
        {
            if(jt->left==it->left&&jt->right.compare(it->right)==0)
            {
                goto dontadd;
            }
        }
        uniq.push_back(*it);
        dontadd:;
    }
   // cout<<"Unique kernels for item "<<item_to_number(I)<<endl;
    //print_item(uniq);
    return uniq;
}

vector<struct rule> kernel_same(vector<vector<struct rule> > canonical_LR_items, vector<struct rule> I)
{
    for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
    {
    		if(!areSame(*it, I))
    		{
		      vector<struct rule> current_item= *it;

		      vector<struct rule>kernel1 = get_unique(current_item);
		      vector<struct rule>kernel2 = get_unique(I);


		      if(kernel1.size()!=kernel2.size())
		          continue;

		      vector<struct rule>::iterator jt2 = kernel2.begin();
		      for(vector<struct rule>::iterator jt = kernel1.begin();jt!=kernel1.end();jt++)
		      {
		          if(jt->left!=jt2->left||jt->right.compare(jt2->right)!=0)
		              goto next_item_please;
		          jt2++;
		      }
		      return current_item;
		      next_item_please:;
		     }
    }
    vector<struct rule> no_item;
    return no_item;
}

int special_item_to_number(vector<struct rule> I)
{
    int item_number = 0;
    for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
    {
        vector<struct rule>kernel1 = get_unique(*it);
		    vector<struct rule>kernel2 = get_unique(I);

		    vector<struct rule>::iterator jt2 = kernel2.begin();
	      for(vector<struct rule>::iterator jt = kernel1.begin();jt!=kernel1.end();jt++)
	      {
	          if(jt->left!=jt2->left||jt->right.compare(jt2->right)!=0)
	              goto special_next_item_please;
	          jt2++;
	      }
	      return item_number;
	      special_next_item_please:
        item_number++;
    }
    return -1; //not a member of the canonical set of items
}


int main()
{
	int n;
	cout<<"Enter number of rules:\n";
	cin>>n;
	n = n+1;
	struct rule r[n];

    set <char> grammar_symbols;

	cout<<"Enter the rules:\n";
	for(int i=1;i<n;i++)
	{
		cin>>r[i].left>>r[i].right;
		grammar_symbols.insert(r[i].left);
		grammar_symbols.insert(r[i].right.begin(), r[i].right.end());
	    if(i==1)
            start_symbol = r[i].left;
	}

	//create the augmented grammar

	r[0].left = 'W';
	//W is the equivalent of S'
	r[0].right.push_back(start_symbol);
	//added the production S'->S
    struct rule item;
	item.left = r[0].left;
	item.right = "." + r[0].right;
	item.lookahead = '$';
    vector<struct rule> start_I;
	start_I.push_back(item);

	 map<char, set<char> > fol;
    map<string, set<char> > fir;

	/*
	TESTING CLOSURE AND GOTO
	vector<struct rule> I0;

    //print_item(start_I);
	I0 = closure(start_I, r, n);

	cout<<"I0 itemset found as below:\n";
	print_item(I0);

    vector<struct rule> goto_I0;
    goto_I0 = goto_function(I0, '(', r, n);

    print_item(goto_I0);
    */


    int new_items_added = 1;
    canonical_LR_items.push_back(closure(start_I, r, n));

    map<pair<int, char>, int> GOTO;
    //the GOTO function has been defined
    int item_number = 1;
    while(new_items_added)
    {
        new_items_added = 0; //assume no new items added. If added, will change.
        for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
        {
            for(set<char>::iterator g = grammar_symbols.begin();g!=grammar_symbols.end();g++)
            {
                vector<struct rule> goto_I_X = goto_function(*it, *g, r, n);

                if(goto_I_X.size()!=0)
                {
                    if(!there(canonical_LR_items, goto_I_X))
                    {
                        cout<<"Found a new item: "<< item_number<<"\n";                         
                        print_item(goto_I_X);

	                      //GOTO[make_pair(item_to_number(*it), *g)] = item_number++;
	                      canonical_LR_items.push_back(goto_I_X);

                        it = canonical_LR_items.begin();
                        new_items_added = 1;
                    }
                    else
                    {
                         //GOTO[make_pair(item_to_number(*it), *g)] = item_to_number(goto_I_X);
                    }
                }
            }
        }
    }

    for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
    {
    			vector<struct rule> current_item = *it;
      		vector<struct rule> same_kernel_item = kernel_same(canonical_LR_items, current_item);
			    if(same_kernel_item.size()!=0)
			    {
			    		int current_item_number = item_to_number(*it);
			        int prev_item_number = item_to_number(same_kernel_item);
			        cout<<"Found item "<<current_item_number<<" to have the same kernel item as item "<<prev_item_number<<endl;
			        print_item(same_kernel_item);
			        canonical_LR_items.erase(canonical_LR_items.begin()+current_item_number);
			        canonical_LR_items.erase(canonical_LR_items.begin()+prev_item_number-1);
			        /*cout<<"Canonical items:\n";
			        for(vector<vector<struct rule> >::iterator jt = canonical_LR_items.begin();jt!=canonical_LR_items.end();jt++)
							{
						    cout<<"Item "<<item_to_number(*jt)<<endl;
							    print_item(*jt);
							}*/
			        //erase the previous un-unioned same kernel item
			        for(vector<struct rule>::iterator st = current_item.begin();st!=current_item.end();st++)
			        {
			            same_kernel_item.push_back(*st);
			        }
			        canonical_LR_items.insert(canonical_LR_items.begin() + current_item_number-1, same_kernel_item);
			        
			        it = canonical_LR_items.begin();
			        //added the union of both the same kernel sets at the same position
			    }
		}
    for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
    {
        cout<<"Item "<<item_to_number(*it)<<endl;
        print_item(*it);
    }
    //canonical itemsets are found!

     for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
        {
            for(set<char>::iterator g = grammar_symbols.begin();g!=grammar_symbols.end();g++)
            {
                vector<struct rule> goto_I_X = goto_function(*it, *g, r, n);

                if(goto_I_X.size()!=0)
                {
                    if(GOTO.find(make_pair(item_to_number(*it), *g))==GOTO.end())
                    {
                        //cout<<"Found a new item: "<< item_to_number(*it)<<"\n";                         
                        //print_item(goto_I_X);

                    		if(special_item_to_number(goto_I_X)!=-1)
	                      GOTO[make_pair(special_item_to_number(*it), *g)] = special_item_to_number(goto_I_X);
	                      //canonical_LR_items.push_back(goto_I_X);

                        //it = canonical_LR_items.begin();
                        //new_items_added = 1;
                    }
                    else
                    {
                    	cout<<"Found that "<<special_item_to_number(*it)<<" and "<<*g<<" has goto of "<<GOTO[make_pair(special_item_to_number(*it), *g)]<<endl;
                    }
                }
            }
          }

    for(map<pair<int, char>, int>::iterator it = GOTO.begin();it!=GOTO.end();it++)
    {
    		//if(!isTerminal(it->first.second, r, n))
        cout<<"Item "<<it->first.first<<" on "<<it->first.second<<": "<<"Item "<<it->second<<endl;
    }

    map<pair<int, char>, struct action_entry> ACTION;
    int item_count = 0;
    for(vector<vector<struct rule> >::iterator it = canonical_LR_items.begin();it!=canonical_LR_items.end();it++)
    {
        vector<struct rule> I= *it;
          cout<<"Considering item "<<item_to_number(I)<<":\n";
        for(vector<struct rule>::iterator vt = I.begin();vt!=I.end();vt++)
        {

            cout<<"Production: "<<vt->left<<"->"<<vt->right<<endl;
            int dot_pos = vt->right.find(".");

            if(dot_pos<vt->right.length()-1)
            {
                //found a production of type A->alpha .abeta
                char a = vt->right[dot_pos+1];
                if(isTerminal(a, r, n))
                {
                    if(GOTO.find(make_pair(item_to_number(I), a))!=GOTO.end())
                    {
                        int j = GOTO[make_pair(item_to_number(I), a)];
                        struct action_entry entry;
                        entry.s_r_a = 's'; //shift
                        entry.state_number = j;
                        if(ACTION.find(make_pair(item_to_number(I),a))==ACTION.end())
                            ACTION[make_pair(item_to_number(I), a)] = entry;
                        else
                        {
                            map<pair<int, char>, struct action_entry>::iterator actt = ACTION.find(make_pair(item_to_number(I),a));
                           if(actt->second.s_r_a=='s')
                            {
                                //shift with shift, not required
                                continue;
                                //cout<<"s"<<actt->second.state_number<<endl;
                            }
                            cout<<"Multiple entries for "<<item_to_number(I)<<", "<<a<<": \n";
                            cout<<"Existing entry: ";

                            cout<<actt->first.first<<", "<<actt->first.second<<": ";

                            if(actt->second.s_r_a=='r')
                            {
                                cout<<"r "<<actt->second.prod.left<<"->"<<actt->second.prod.right<<endl;
                            }
                            else
                            {
                                cout<<"acc"<<endl;
                            }

                            cout<<"New entry: ";
                            cout<<"s "<<entry.state_number<<endl;

                            int option;
                            cout<<"Replace? : ";
                            cin>>option;

                            if(option==1)
                            {
                                ACTION[make_pair(item_to_number(I), a)] = entry;
                            }
                        }

                    }
                }
            }
            else
            {
                //found a production of type A->alpha.
                struct action_entry entry;
                if(vt->left=='W')
                {
                    //found W->S, $. This means accept
                    entry.s_r_a = 'a';
                    ACTION[make_pair(item_to_number(I), '$')] = entry; //accept
                    continue;
                }
                char a = vt->lookahead;

                entry.s_r_a = 'r'; //reduce
                entry.prod.left = vt->left;
                entry.prod.right = vt->right;

                entry.prod.right.erase(dot_pos);

                if(ACTION.find(make_pair(item_to_number(I),a))==ACTION.end())
                    ACTION[make_pair(item_to_number(I), a)] = entry;
                else
                {
                    map<pair<int, char>, struct action_entry>::iterator actt = ACTION.find(make_pair(item_to_number(I),a));
                    cout<<"Multiple entries for "<<item_to_number(I)<<", "<<a<<": \n";
                    cout<<"Existing entry: ";

                    cout<<actt->first.first<<", "<<actt->first.second<<": ";
                    if(actt->second.s_r_a=='s')
                    {
                        cout<<"s"<<actt->second.state_number<<endl;
                    }
                    else if(actt->second.s_r_a=='r')
                    {
                        cout<<"r "<<actt->second.prod.left<<"->"<<actt->second.prod.right<<endl;
                    }
                    else
                    {
                        cout<<"acc"<<endl;
                    }

                    cout<<"New entry: ";
                    cout<<"r "<<entry.prod.left<<"->"<<entry.prod.right<<endl;

                    int option;
                    cout<<"Replace? : ";
                    cin>>option;

                    if(option==1)
                    {
                        ACTION[make_pair(item_to_number(I), a)] = entry;
                    }
                }
            }
        }
    }
    for(map<pair<int, char>, struct action_entry>::iterator it = ACTION.begin();it!=ACTION.end();it++)
    {
            cout<<it->first.first<<", "<<it->first.second<<": ";
            if(it->second.s_r_a=='s')
            {
                cout<<"s"<<it->second.state_number<<endl;
            }
            else if(it->second.s_r_a=='r')
            {
                cout<<"r "<<it->second.prod.left<<"->"<<it->second.prod.right<<endl;
            }
            else
            {
                cout<<"acc"<<endl;
            }
    }

    while(1)
    {string w;
    cout<<"Enter input string: ";
    cin>>w;

    w = w+ "$";
    int inp = 0;
    char a = w[inp];

    vector<int> states;
    states.push_back(0);
    vector<char> symbols;
    while(1)
    {

        cout<<"States:";
        for(vector<int>::iterator it= states.begin();it!=states.end();it++)
        cout<<*it<<" ";
        cout<<"----";
        //cout<<"Sentential Form:";
        for(vector<char>::iterator it= symbols.begin();it!=symbols.end();it++)
        {
            cout<<*it<<" ";
        }
        cout<<"---------------";
        //cout<<"Action:";
        int tos = states[states.size()-1];
        a = w[inp];

        //cout<<tos<<" "<<a<<endl;
        if(ACTION.find(make_pair(tos, a))==ACTION.end())
        {
            cout<<"ERROR parsing, exiting\n";
            return 1;
        }
        else
        {
            struct action_entry entry = ACTION[make_pair(tos, a)];

            if(entry.s_r_a=='s')
            {
                cout<<"Shift "<<a<<endl;
                inp++;
                symbols.push_back(a);
                states.push_back(entry.state_number);
            }
            else if(entry.s_r_a=='r')
            {
                cout<<"Reduce "<<entry.prod.left<<"->"<<entry.prod.right<<endl;
                for(int i=0;i<entry.prod.right.length();i++)
                {
                    symbols.pop_back();
                    states.pop_back();
                }
                symbols.push_back(entry.prod.left);

                int top_states = states[states.size()-1];
                char top_symbols = symbols[symbols.size()-1];
                states.push_back(GOTO[make_pair(top_states, top_symbols)]);

            }
            else
            {
                cout<<"ACCEPT!!\n";
                break;
            }
        }
    }
    }
    
}

/*
GRAMMAR FOR LR0 PARSING AS GIVEN IN QUESTION:
4
E E+E
E E*E
E (E)
E i

//During conflicts, provide 0 to retain original info, 1 to replace.

3
S CC
C cC
C d

6
E E+T
E T
T T*F
T F
F (E)
F i

6
S aAd
S bBd
S aBe
S bAe
A c
B c


*/
