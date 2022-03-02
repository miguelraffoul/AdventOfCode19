#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<int> getIntCodeProgram( void );

int main( void ) {
  vector<int> int_code = getIntCodeProgram();
  int curr_op = 0;
  
  int_code[1] = 12;
  int_code[2] = 2;
  while( int_code[curr_op] != 99 ) {
    switch( int_code[curr_op] ) {
      case 1:
        int_code[int_code[curr_op + 3]] = int_code[int_code[curr_op + 1]] + int_code[int_code[curr_op + 2]];
        break;
      case 2:
        int_code[int_code[curr_op + 3]] = int_code[int_code[curr_op + 1]] * int_code[int_code[curr_op + 2]];
        break;
    }
    curr_op += 4;
  }
  cout << int_code[0] << endl;
  return EXIT_SUCCESS;
}

vector<int> getIntCodeProgram( void ) {
  string input;
  cin >> input;
  for( unsigned int x = 0; x < input.size(); ++x ) {
    if( input[x] == ',' )
      input[x] = ' ';
  }

  vector<int> int_code;
  istringstream iss( input );
  int op;
  while( iss >> op ) {
    int_code.push_back( op );
  }
  return int_code;
}