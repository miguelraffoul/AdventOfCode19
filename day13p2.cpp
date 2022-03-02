#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "IntCodeComputer.cpp"

using namespace std;

vector<long long int> getIntCodeProgram( void );
string coordsToStrig( const int&, const int& );
void printPanels( unordered_map<string, long long int>&, const int&, 
                  const int&, const int&, const int& );

int main( void ) {
  vector<long long int> program = getIntCodeProgram();
  program[0] = 2;
  
  IntCodeComputer comp( program );
  unordered_map<string, long long int> panels;
  deque<long long int> inputs, outputs;
  int min_x = 0x7fffffff, max_x = 0;
  int min_y = 0x7fffffff, max_y = 0;
  int paddle_x_pos, ball_x_pos;
  int score = 0;
  while( comp.getCurrentState() != IntCodeComputer::FINISHED ) {
    outputs = comp.executeIntCode( inputs );
    for( unsigned int p = 0; p < outputs.size(); p += 3 ) {
      if( outputs[p] == -1 && outputs[p + 1] == 0 ) 
        score = outputs[p + 2];
      else {
        panels[coordsToStrig( outputs[p + 1], outputs[p] )] = outputs[p + 2];
        min_y = outputs[p + 1] < min_y ? outputs[p + 1] : min_y;
        max_y = outputs[p + 1] > max_y ? outputs[p + 1] : max_y;
        min_x = outputs[p] < min_x ? outputs[p] : min_x;
        max_x = outputs[p] > max_x ? outputs[p] : max_x;
        if( outputs[p + 2] == 3 )
          paddle_x_pos = outputs[p];
        if( outputs[p + 2] == 4 )
          ball_x_pos = outputs[p];
      }
    }
    system( "clear" );
    printPanels( panels, min_y, max_y, min_x, max_x );
    cout << "Score: " << score << endl;
    if( paddle_x_pos < ball_x_pos )   
      inputs.push_back( 1 );
    else if( paddle_x_pos > ball_x_pos )   
      inputs.push_back( -1 );
    else
      inputs.push_back( 0 );
    this_thread::sleep_for( chrono::milliseconds( 50 ) );
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

string coordsToStrig( const int& y, const int& x ) {
  return to_string( y ) + ":"  + to_string( x );
}



void printPanels( unordered_map<string, long long int>& panels, const int& min_y, 
                  const int& max_y, const int& min_x, const int& max_x ) {
  string id;
  for( int y = max_y; y >= min_y; --y ) {
    for( int x = min_x; x <= max_x; ++x ) {
      id = coordsToStrig( y, x );
      if( panels.find( id ) != panels.end() ) {
        if( panels[id] == 0 )
          cout << ' ';
        else if( panels[id] == 1 )
          cout << '@';
        else if( panels[id] == 2 )
          cout << '#';
        else if( panels[id] == 3 )
          cout << '=';
        else if( panels[id] == 4 )
          cout << '*';
      }
      else 
        cout << ' ';
    }
    cout << endl;
  }
}