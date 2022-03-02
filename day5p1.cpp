#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<int> getIntCodeProgram( void );
string intToReverseString( const int& );

int main( void ) {
  vector<int> int_code = getIntCodeProgram();
  int inst_ptr = 0;
  int inst_inc;
  
  while( int_code[inst_ptr] != 99 ) {
    string inst = intToReverseString( int_code[inst_ptr] );
    char op_code = inst[0];
    char p1_mode = inst.size() >= 3 ? inst[2] : '-';
    char p2_mode = inst.size() >= 4 ? inst[3] : '-';
    char p3_mode = inst.size() >= 5 ? inst[4] : '-';
    int p1, p2;
    switch( op_code ) {
      case '1':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        p2 = p2_mode == '1' ? int_code[inst_ptr + 2] : int_code[int_code[inst_ptr + 2]];
        int_code[int_code[inst_ptr + 3]] = p1 + p2;
        inst_inc = 4;
        break;
      case '2':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        p2 = p2_mode == '1' ? int_code[inst_ptr + 2] : int_code[int_code[inst_ptr + 2]];
        int_code[int_code[inst_ptr + 3]] = p1 * p2;
        inst_inc = 4;
        break;
      case '3':
        cout << "\\> ";
        cin >> int_code[inst_ptr + 1];
        inst_inc = 2;
        break;
      case '4':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        cout << "program output: "  << p1 << endl;
        inst_inc = 2;
        break; 
    }
    inst_ptr += inst_inc;
  }
  return EXIT_SUCCESS;
}

vector<int> getIntCodeProgram( void ) {
  ifstream ifs;
  string input;
  ifs.open( "./day5input", ifstream::in );
  ifs >> input;
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
  ifs.close();
  return int_code;
}

string intToReverseString( const int& inst ) {
  string inst_str = to_string( inst );
  return string( inst_str.rbegin(), inst_str.rend() );
}