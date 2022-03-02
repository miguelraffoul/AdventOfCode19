#include <deque>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class IntCodeComputer {
  public:
  enum computerState {
    NEW_COMPUTER      = 0,
    COMPUTER_RUNNING  = 1,
    FINISHED          = 2,
  };

  IntCodeComputer( const vector<long long int>& _int_code ) :
    int_code( _int_code ),
    current_state( NEW_COMPUTER ),
    inst_ptr( 0 ),
    relative_base( 0 )
  {
  }

  int getCurrentState( void ) {
    return current_state;
  }

  deque<long long int> executeIntCode( deque<long long int>& inputs ) {
    deque<long long int> outputs;  
    current_state = COMPUTER_RUNNING;  
    while( int_code[inst_ptr] != 99 ) {
      string inst = intToReverseString( int_code[inst_ptr] );
      char op_code = inst[0];
      char p1_mode = inst.size() >= 3 ? inst[2] : '-';
      char p2_mode = inst.size() >= 4 ? inst[3] : '-';
      char p3_mode = inst.size() >= 5 ? inst[4] : '-';
      long long int p1, p2, p3;
      switch( op_code ) {
        case '1':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          p2 = getParam( p2_mode, inst_ptr + 2 );
          p3 = getWriteParam( p3_mode, inst_ptr + 3 );
          write( p3, p1 + p2 );
          inst_ptr += 4;
          break;
        case '2':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          p2 = getParam( p2_mode, inst_ptr + 2 );
          p3 = getWriteParam( p3_mode, inst_ptr + 3 );
          write( p3, p1 * p2 );
          inst_ptr += 4;
          break;
        case '3':
          if( inputs.size() > 0 ) {
            p1 = getWriteParam( p1_mode, inst_ptr + 1 );
            write( p1, inputs.front() );
            inputs.pop_front();
            inst_ptr += 2;
          }
          else {
            return outputs;
          }
          break;
        case '4':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          outputs.push_back( p1 );
          inst_ptr += 2;
          break; 
        case '5':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          p2 = getParam( p2_mode, inst_ptr + 2 );
          if( p1 != 0 )
            inst_ptr = p2;
          else
            inst_ptr += 3;
          break;
        case '6':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          p2 = getParam( p2_mode, inst_ptr + 2 );
          if( p1 == 0 )
            inst_ptr = p2;
          else
            inst_ptr += 3;
          break;
        case '7':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          p2 = getParam( p2_mode, inst_ptr + 2 );
          p3 = getWriteParam( p3_mode, inst_ptr + 3 );
          write( p3, (p1 < p2 ? 1 : 0) );
          inst_ptr += 4;
          break;
        case '8':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          p2 = getParam( p2_mode, inst_ptr + 2 );
          p3 = getWriteParam( p3_mode, inst_ptr + 3 );
          write( p3, (p1 == p2 ? 1 : 0) );
          inst_ptr += 4;
          break;
        case '9':
          p1 = getParam( p1_mode, inst_ptr + 1 );
          relative_base += p1;
          inst_ptr += 2;
          break;
      }
    }
    current_state = FINISHED;
    return outputs;
  }

  private:
  vector<long long int> int_code;
  unordered_map<long long int, long long int> memory;
  int current_state;
  int inst_ptr;
  long long int relative_base;

  string intToReverseString( const long long int& inst ) {
    string inst_str = to_string( inst );
    return string( inst_str.rbegin(), inst_str.rend() );
  }

  long long int read( const long long int& address ) {
    if( address < int_code.size() )
      return int_code[address]; 
    else {
      if( memory.find( address ) == memory.end() ) {
        memory[address] = 0;
      }
      return memory[address];
    }
  }

  void write( const long long int& address, const long long& value ) {
    if( address < int_code.size() )
      int_code[address] = value;
    else
      memory[address] = value;
  }

  long long int getParam( const char& p_mode, const long long int& address ) {
    switch( p_mode ) {
      case '0':
        return read( read( address ) );
      case '1':
        return read( address );
      case '2':
        return read( read( address ) + relative_base );
    }
    return read( read( address ) );
  }

  long long int getWriteParam( const char& p_mode, const long long int& address ) {
    switch( p_mode ) {
      case '0':
        return read( address );
      case '2':
        return read( address ) + relative_base;
    }
    return read( address );
  }
};