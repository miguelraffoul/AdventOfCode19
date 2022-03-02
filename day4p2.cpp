#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool isValidPassword( const string& );

int main( void ) {
  int counter = 0;
  for( int p = 254032; p <= 789860; ++p ) {
    if( isValidPassword( to_string( p ) ) ) {
      ++counter;
      //cout << p << endl;
    }
  }
  cout << counter << endl;
  return EXIT_SUCCESS;
}

bool isValidPassword( const string& passwd ) {
  bool contains_double =  false;
  vector<char> digit_stack;
  digit_stack.push_back( passwd[0] );
  for( unsigned int c = 1; c < passwd.size(); ++c ) {
    if( passwd[c] < passwd[c - 1] )
      return false;
    if( passwd[c] != passwd[c - 1] ) {
      if( digit_stack.size() == 2 )
        contains_double = true;
      digit_stack.clear();
    }
    digit_stack.push_back( passwd[c] );
  }
  if( digit_stack.size() == 2 )
    contains_double = true;
  return contains_double;
}


//254032-789860