#include <cstdlib>
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
int getUsedOREAmt( unordered_map<string, Reaction>&, unordered_map<string, int>&, const string& );

int main( void ) {
  unordered_map<string, Reaction> reactions_table;
  unordered_map<string, int> available_chems;

  reactions_table = getReactionTable();
  //printReactionsTable( reactions_table ); cout << "=============" << endl;
  cout << getUsedOREAmt( reactions_table, available_chems, string( "FUEL" ) ) << endl;

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

int getUsedOREAmt( unordered_map<string, Reaction>& reactions_tab, unordered_map<string, int>& available_chems, const string& target ) {
  Reaction react = reactions_tab[target];
  int used_ore_amt = 0;
  for( unsigned int ic = 0; ic < react.input_chems.size(); ++ic ) {
    if( react.input_chems[ic].first == "ORE" ) {
      used_ore_amt += react.input_chems[ic].second;
    }
    else if( available_chems.find( react.input_chems[ic].first ) !=  available_chems.end() &&
        available_chems[react.input_chems[ic].first] >= react.input_chems[ic].second ) {
      available_chems[react.input_chems[ic].first] -= react.input_chems[ic].second;
    }
    else {
      do {
        used_ore_amt += getUsedOREAmt( reactions_tab, available_chems, react.input_chems[ic].first );
      } while( available_chems[react.input_chems[ic].first] < react.input_chems[ic].second );
      available_chems[react.input_chems[ic].first] -= react.input_chems[ic].second;
    }
  }
  if( available_chems.find( react.output_chem ) == available_chems.end() )
    available_chems[react.output_chem] = 0;
  available_chems[react.output_chem] += react.output_amt;
  return used_ore_amt;
}