#include <cstdlib>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "IntCodeComputer.cpp"

using namespace std;

vector<long long int> getIntCodeProgram( void );

int main( void ) {
  IntCodeComputer comp( getIntCodeProgram() );
  deque<long long int> inputs, outputs;
  //iputs.push_back( 1 ); //part 1
  inputs.push_back( 2 ); //part 2
  outputs = comp.executeIntCode( inputs );
  for( unsigned int x = 0; x < outputs.size(); ++x ) {
    cout << outputs[x] << endl;
  }
  return EXIT_SUCCESS;
}

vector<long long int> getIntCodeProgram( void ) {
  string input;
  cin >> input;
  for( unsigned int x = 0; x < input.size(); ++x ) {
    if( input[x] == ',' )
      input[x] = ' ';
  }
  vector<long long int> int_code;
  istringstream iss( input );
  long long int op;
  while( iss >> op ) {
    int_code.push_back( op );
  }
  return int_code;
}