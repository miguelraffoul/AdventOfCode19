#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<int> getIntCodeProgram( void );

int main( void ) {
  vector<int> backup = getIntCodeProgram();
  vector<int> int_code;
  int noun = 0, verb = 0, inst_ptr = 0;
  
  while( true ) {
    int_code = backup;
    inst_ptr = 0;
    int_code[1] = noun;
    int_code[2] = verb;
    while( int_code[inst_ptr] != 99 ) {
      switch( int_code[inst_ptr] ) {
        case 1:
          int_code[int_code[inst_ptr + 3]] = int_code[int_code[inst_ptr + 1]] + int_code[int_code[inst_ptr + 2]];
          break;
        case 2:
          int_code[int_code[inst_ptr + 3]] = int_code[int_code[inst_ptr + 1]] * int_code[int_code[inst_ptr + 2]];
          break;
      }
      inst_ptr += 4;
    }
    if( int_code[0] == 19690720 )
      break;
    if( verb < 99 )
      ++verb;
    else {
      verb = 0;
      ++noun;
    }
  }
  cout << ((100 * noun) + verb) << endl;
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