#include <bitset>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<int> getIntCodeProgram( void );
string intToReverseString( const int& );
int getThrusterHighestSignal( const vector<int>&, bitset<5>&, const int& );
int executeIntCode( vector<int>, vector<int>& );

int main( void ) {
  vector<int> int_code = getIntCodeProgram();
  bitset<5> available_specs( string( "11111" ) );
  cout << getThrusterHighestSignal( int_code, available_specs, 0 ) << endl;
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

string intToReverseString( const int& inst ) {
  string inst_str = to_string( inst );
  return string( inst_str.rbegin(), inst_str.rend() );
}

int getThrusterHighestSignal( const vector<int>& int_code, bitset<5>& available_specs, const int& prev_signal ) {
  int int_code_output, next_signal, highest_signal = 0;
  vector<int> int_code_inputs;
  for( unsigned int x = 0; x < available_specs.size(); ++x ) {
    if( available_specs.test( x ) ) {
      available_specs.flip( x );
      int_code_inputs.push_back( prev_signal );
      int_code_inputs.push_back( x );
      int_code_output = executeIntCode( int_code, int_code_inputs );
      if( available_specs.count() > 0 ) {
        next_signal = getThrusterHighestSignal( int_code, available_specs, int_code_output );
        highest_signal = next_signal > highest_signal ? next_signal : highest_signal;
      }
      else {
        highest_signal = int_code_output;
      }
      available_specs.flip( x );
    }
  }
  return highest_signal;
}

int executeIntCode( vector<int> int_code, vector<int>& inputs ) {
  int output, inst_ptr = 0;
  
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
        inst_ptr += 4;
        break;
      case '2':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        p2 = p2_mode == '1' ? int_code[inst_ptr + 2] : int_code[int_code[inst_ptr + 2]];
        int_code[int_code[inst_ptr + 3]] = p1 * p2;
        inst_ptr += 4;
        break;
      case '3':
        int_code[int_code[inst_ptr + 1]] = inputs.back();
        inputs.pop_back();
        inst_ptr += 2;
        break;
      case '4':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        output = p1;
        inst_ptr += 2;
        break; 
      case '5':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        p2 = p2_mode == '1' ? int_code[inst_ptr + 2] : int_code[int_code[inst_ptr + 2]];
        if( p1 != 0 )
          inst_ptr = p2;
        else
          inst_ptr += 3;
        break;
      case '6':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        p2 = p2_mode == '1' ? int_code[inst_ptr + 2] : int_code[int_code[inst_ptr + 2]];
        if( p1 == 0 )
          inst_ptr = p2;
        else
          inst_ptr += 3;
        break;
      case '7':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        p2 = p2_mode == '1' ? int_code[inst_ptr + 2] : int_code[int_code[inst_ptr + 2]];
        int_code[int_code[inst_ptr + 3]] = p1 < p2 ? 1 : 0;
        inst_ptr += 4;
        break;
      case '8':
        p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
        p2 = p2_mode == '1' ? int_code[inst_ptr + 2] : int_code[int_code[inst_ptr + 2]];
        int_code[int_code[inst_ptr + 3]] = p1 == p2 ? 1 : 0;
        inst_ptr += 4;
        break;
    }
  }
  return output;
}