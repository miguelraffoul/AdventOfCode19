#include <bitset>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class IntCodeComputer {
  public:
  enum computerState {
    NEW_COMPUTER      = 0,
    COMPUTER_RUNNING  = 1,
    FINISHED          = 2,
  };

  IntCodeComputer( const vector<int>& _int_code ) :
    int_code( _int_code ),
    current_state( NEW_COMPUTER ),
    inst_ptr( 0 )
  {
  }

  int getCurrentState( void ) {
    return current_state;
  }

  string intToReverseString( const int& inst ) {
    string inst_str = to_string( inst );
    return string( inst_str.rbegin(), inst_str.rend() );
  }

  deque<int> executeIntCode( deque<int>& inputs ) {
    deque<int> outputs;  
    current_state = COMPUTER_RUNNING;  
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
          if( inputs.size() > 0 ) {
            int_code[int_code[inst_ptr + 1]] = inputs.front();
            inputs.pop_front();
            inst_ptr += 2;
          }
          else {
            return outputs;
          }
          break;
        case '4':
          p1 = p1_mode == '1' ? int_code[inst_ptr + 1] : int_code[int_code[inst_ptr + 1]];
          outputs.push_back( p1 );
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
    current_state = FINISHED;
    return outputs;
  }

  private:
  vector<int> int_code;
  int current_state;
  int inst_ptr;
};

vector<int> getIntCodeProgram( void );
int getThrusterHighestSignal( const vector<int>&, bitset<5>&, deque<int> );

int main( void ) {
  vector<int> int_code = getIntCodeProgram();
  bitset<5> available_phases( string( "11111" ) );
  deque<int> phases;
  cout << getThrusterHighestSignal( int_code, available_phases, phases ) << endl;
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

int getThrusterHighestSignal( const vector<int>& int_code, bitset<5>& available_phases, deque<int> phases ) {
  int next_signal, highest_signal = 0;
  for( unsigned int x = 0; x < available_phases.size(); ++x ) {
    if( available_phases.test( x ) ) {
      available_phases.flip( x );
      phases.push_back( x + 5 );
      if( available_phases.count() > 0 ) {
        next_signal = getThrusterHighestSignal( int_code, available_phases, phases );
        highest_signal = next_signal > highest_signal ? next_signal : highest_signal;
        phases.pop_back();
      } 
      else {
        vector<IntCodeComputer> amplifiers;
        deque<int> inputs;
        bool computers_running = true;
        for( unsigned int a = 0; a < available_phases.size(); ++a ) {
          IntCodeComputer amp( int_code );
          amplifiers.push_back( amp );
        }
        if( inputs.size() == 0 ) {
          inputs.push_back( 0 );
        }
        while( computers_running ) {
          computers_running = false;
          for( unsigned int a = 0; a < amplifiers.size(); ++a ) {
            if( amplifiers[a].getCurrentState() != IntCodeComputer::FINISHED ) {
              computers_running = true;
              if( amplifiers[a].getCurrentState() == IntCodeComputer::NEW_COMPUTER ) {
                inputs.push_front( phases.front() );
                phases.pop_front();
              }
              inputs = amplifiers[a].executeIntCode( inputs );
            }
          }
        }
        highest_signal = inputs.back();
      }
      available_phases.flip( x );
    }
  }
  return highest_signal;
}

