#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "IntCodeComputer.cpp"

using namespace std;

vector<long long int> getIntCodeProgram( void );
string coordsToStrig( const int&, const int& );
void turnLeft( char& );
void turnRight( char& );
void chagePosition( const char&, int&, int& );
void printPanels( unordered_map<string, long long int>&, const int&, 
                  const int&, const int&, const int& );

int main( void ) {
  IntCodeComputer comp( getIntCodeProgram() );
  unordered_map<string, long long int> panels;
  deque<long long int> inputs, outputs;
  int min_y = 0, min_x = 0;
  int max_y = 0, max_x = 0;
  int y = 0, x = 0;
  char dir = '^';
  /********part 2*******/
  panels["0:0"] = 1;
  /*********************/
  while( comp.getCurrentState() != IntCodeComputer::FINISHED ) {
    string id = coordsToStrig( y, x );
    if( panels.find( id ) == panels.end() ) {
      panels[id] = 0;
    }
    inputs.push_back( panels[id] );
    outputs = comp.executeIntCode( inputs );
    panels[id] = outputs.front();
    outputs.pop_front();
    if( outputs.front() == 0 )
      turnLeft( dir );
    else
      turnRight( dir );
    outputs.pop_front();
    chagePosition( dir, y, x );
    max_y = y > max_y ? y : max_y;
    min_y = y < min_y ? y : min_y;
    max_x = x > max_x ? x : max_x;
    min_x = x < min_x ? x : min_x;
  } 
  printPanels( panels, min_y, max_y, min_x, max_x );
  cout << panels.size() << endl;
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

void turnLeft( char& dir ) {
  switch( dir ) {
    case '^':
      dir = '<';
      break;
    case '<':
      dir = 'v';
      break;
    case 'v':
      dir = '>';
      break;
    case '>':
      dir = '^';
      break;
  }
}

void turnRight( char& dir ) { 
  switch( dir ) {
    case '^':
      dir = '>';
      break;
    case '>':
      dir = 'v';
      break;
    case 'v':
      dir = '<';
      break;
    case '<':
      dir = '^';
      break;
  }
}

void chagePosition( const char& dir, int& y, int& x ) {
  switch( dir ) {
    case '^':
      ++y;
      break;
    case '>':
      ++x;
      break;
    case 'v':
      --y;
      break;
    case '<':
      --x;
      break;
  }
}

void printPanels( unordered_map<string, long long int>& panels, const int& min_y, 
                  const int& max_y, const int& min_x, const int& max_x ) {
  string id;
  for( int y = max_y; y >= min_y; --y ) {
    for( int x = min_x; x <= max_x; ++x ) {
      id = coordsToStrig( y, x );
      if( panels.find( id ) != panels.end() )
        cout << (panels[id] == 0 ? '.' : '#');
      else 
        cout << '.';
    }
    cout << endl;
  }
}