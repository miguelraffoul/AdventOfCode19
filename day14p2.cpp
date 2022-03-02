#include <cstdlib>
#include <cmath>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Reaction {
  string output_chem;
  int output_amt;
  vector< pair<string, int> > input_chems;
};

unordered_map<string, Reaction> getReactionTable();
void printReactionsTable( unordered_map<string, Reaction>& );
long long int getUsedOREAmt( unordered_map<string, Reaction>&, const pair<string, long long int>& );

int main( void ) {
  unordered_map<string, Reaction> reactions_table;
  reactions_table = getReactionTable();
  //printReactionsTable( reactions_table ); cout << "=============" << endl;
  
  long long int used_ore;
  long long int min = 1000000000000 / getUsedOREAmt( reactions_table, make_pair( "FUEL", 1 ) );
  long long int max = min * 2;
  long long int last_valid;
  while( min <= max ) {
    long long int middle = min + ((max - min) / 2);
    used_ore = getUsedOREAmt( reactions_table, make_pair( "FUEL", middle ) );
    if( used_ore > 1000000000000 )
      max = middle - 1;
    if( used_ore < 1000000000000 ) {
      last_valid = middle;
      min = middle + 1;
    }
  }
  cout << last_valid << endl;

  return EXIT_SUCCESS;
}

unordered_map<string, Reaction> getReactionTable( void ) {
  unordered_map<string, Reaction> react_tab;
  string input_react;
  while( getline( cin, input_react) ) {
    for( unsigned int c = 0; c < input_react.size(); ++c ) {
      if( input_react[c] == '=' || input_react[c] == '>' || input_react[c] == ',' )
        input_react[c] = ' ';
    }
    istringstream iss( input_react );
    string tmp1, tmp2;
    Reaction react;
    while( iss >> tmp1 ) {
      if( tmp1[0] >= '0' && tmp1[0] <= '9' ) {
        iss >> tmp2;
        react.output_amt = stoi( tmp1 );
        react.output_chem = tmp2;
        react.input_chems.push_back( make_pair( tmp2, stoi( tmp1 ) ) );
      }
      else {
        react.output_amt = 1;
        react.output_chem = tmp1;
        react.input_chems.push_back( make_pair( tmp1, 1 ) );
      }
    }
    react.input_chems.pop_back();
    react_tab[react.output_chem] = react;
  }
  return react_tab;
}

void printReactionsTable( unordered_map<string, Reaction>& reactions_tab ) {
  unordered_map<string, Reaction>::iterator it;
  for( it = reactions_tab.begin(); it != reactions_tab.end(); ++it ){
    cout << it->first << ":  ";
    for( unsigned int ic = 0; ic < it->second.input_chems.size(); ++ic ) {
      cout << it->second.input_chems[ic].second << " " << it->second.input_chems[ic].first << ", ";
    }
    cout << " => " << it->second.output_amt << " " << it->second.output_chem  << endl;
  }
}

long long int getUsedOREAmt( unordered_map<string, Reaction>& reactions_tab, const pair<string, long long int>& boostrap ) {
  unordered_map<string, long long int> available_chems;
  deque< pair<string, long long int> > due_reactions;
  due_reactions.push_back( boostrap ); 
  long long int used_ore_amt = 0;
  while( due_reactions.size() > 0 ) {
    pair<string, long long int> target = due_reactions.front();
    due_reactions.pop_front();
    Reaction react = reactions_tab[target.first];
    if( available_chems.find( target.first ) == available_chems.end() )
      available_chems[target.first] = 0;

    if( target.first == "ORE" ) {
      used_ore_amt += target.second;
    }
    else if( available_chems[target.first] >= target.second ) {
      available_chems[target.first] -= target.second;
    }
    else {
      long long int needed_amt = target.second - available_chems[target.first];
      long long int multiplier = ceil( ((long double)needed_amt / (long double)react.output_amt) );
      for( unsigned int ic = 0; ic < react.input_chems.size(); ++ic ) {
        due_reactions.push_back( make_pair( react.input_chems[ic].first, (react.input_chems[ic].second * multiplier) ) );
      }
      long long int chem_surplus = (react.output_amt * multiplier) - needed_amt;
      available_chems[target.first] = chem_surplus;
    }
  }
  return used_ore_amt;
}