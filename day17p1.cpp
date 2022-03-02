#include <cstdlib>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "IntCodeComputer.cpp"

vector<long long int> getIntCodeProgram( void );
bool isIntersection( const vector<string>&, const unsigned int&, const unsigned int& );

int main( void ) {
  vector<long long int> program = getIntCodeProgram();
  IntCodeComputer comp( program );
  
  deque<long long int> inputs, outputs;
  vector<string> grid;
  string row;
  while( comp.getCurrentState() != IntCodeComputer::FINISHED ) {
    outputs = comp.executeIntCode( inputs );
    for( unsigned int p = 0; p < outputs.size(); ++p ) {
      if( outputs[p] != 10 ) {
        row += (char)outputs[p];
      }
      else {
        if( row.size() > 0 )
          grid.push_back( row );
        row.clear();
      }
    }
  }
  
  int alignment_sum = 0;
  for( unsigned int r = 0; r < grid.size(); ++r ) {
    for( unsigned int c = 0; c < grid[r].size(); ++c ){
      if( r > 0 && r < (grid.size() - 1) && c > 0 && c < (grid[r].size() - 1) ) {
        if( grid[r][c] == '#' && isIntersection( grid, r, c ) ) {
          grid[r][c] = 'O';
          alignment_sum += (r * c);
        }
      }
      cout << grid[r][c];
    }
    cout << endl;
  }
  cout << alignment_sum << endl;

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

bool isIntersection( const vector<string>& grid, const unsigned int& row, const unsigned int& col ) {
  if( grid[row + 1][col] != '#' )
    return false;
  if( grid[row - 1][col] != '#' )
    return false;
  if( grid[row][col + 1] != '#' )
    return false;
  if( grid[row][col - 1] != '#' )
    return false;
  return true;
}