#include <cstdlib>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "IntCodeComputer.cpp"

#define NONE 0
#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

using namespace std;

struct BFSNode {
  int y_pos;
  int x_pos;
  int distance;
  string pos_str;

  BFSNode( const int& y, const int& x, const int& d, const string& ps ) :
    y_pos( y ),
    x_pos( x ),
    distance( d ),
    pos_str( ps )
  {
  }
};

static uint32_t mem_alloc_count = 0;

vector<long long int> getIntCodeProgram( void );
int getNextStep( unordered_map<string, char>&, const int&, const int& );
string coordsToStrig( const int&, const int& );
pair<int, int> getPositionChange( const int& );
int getOppositeStep( const int& );
void updateGridDimensions( int&, int&, int&, int&, const int&, const int& );
void printGrid( unordered_map<string, char>&, const int&, const int&, const int&, const int&, const string& );
pair<int, int> breadthFirstSearch( unordered_map<string, char>& );
int fillGridWithOxigen( unordered_map<string, char>&, const int&, const int& );

void* operator new( size_t size ) {
  ++mem_alloc_count;
  return malloc( size );
}

int main( void ) {
  vector<long long int> program = getIntCodeProgram();
  IntCodeComputer comp( program );
  deque<long long int> inputs, outputs;

  unordered_map<string, char> known_grid;
  vector<int> droid_steps;
  known_grid["0:0"] = '.';
  int min_x = 0, max_x = 0;
  int min_y = 0, max_y = 0; 
  int droid_y = 0, droid_x = 0;
  int next_step;
  while( comp.getCurrentState() != IntCodeComputer::FINISHED ) {
    //system( "clear" );
    next_step = getNextStep( known_grid, droid_y, droid_x );
    if( next_step != NONE ) { //there are available moves
      inputs.push_back( next_step );
      outputs = comp.executeIntCode( inputs );
      pair<int, int> pc = getPositionChange( next_step );
      switch( outputs[0] ) {
        case 0:
          known_grid[coordsToStrig( (droid_y + pc.first), (droid_x + pc.second) )] = '#';
          updateGridDimensions( min_y, max_y, min_x, max_x, (droid_y + pc.first), (droid_x + pc.second) );
          break;
        case 1:
          droid_steps.push_back( next_step );
          droid_y += pc.first;
          droid_x += pc.second;
          known_grid[coordsToStrig( droid_y, droid_x)] = ' ';
          updateGridDimensions( min_y, max_y, min_x, max_x, droid_y, droid_x );
          break;
        case 2:
          droid_steps.push_back( next_step );
          droid_y += pc.first;
          droid_x += pc.second;
          known_grid[coordsToStrig( droid_y, droid_x)] = '*';
          updateGridDimensions( min_y, max_y, min_x, max_x, droid_y, droid_x );
          break;
      }
    }
    else { //no available moves, backtrack
      next_step = getOppositeStep( droid_steps.back() );
      droid_steps.pop_back();
      inputs.push_back( next_step );
      outputs = comp.executeIntCode( inputs );
      pair<int, int> pc = getPositionChange( next_step );
      droid_y += pc.first;
      droid_x += pc.second;
    }
    //printGrid( known_grid, min_y, max_y, min_x, max_x, coordsToStrig( droid_y, droid_x ) );
    //this_thread::sleep_for( chrono::milliseconds( 50 ) );
  }
  printGrid( known_grid, min_y, max_y, min_x, max_x, coordsToStrig( droid_y, droid_x ) );
  cout << "----- Part 1 -----" << endl;
  cout << "Droid position: "<< droid_y << " : " << droid_x << endl;
  pair<int, int> oxigen_system = breadthFirstSearch( known_grid );
  cout << "----- Part 2 -----" << endl;
  cout << "Oxigen system position: " << oxigen_system.first << " : " << oxigen_system.second << endl;
  cout << "Time to fill system with oxigen: " << fillGridWithOxigen( known_grid, oxigen_system.first, oxigen_system.second ) << endl;
  cout << mem_alloc_count << endl;
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

int getNextStep( unordered_map<string, char>& known_grid, const int& droid_y, const int& droid_x ) {
  if( known_grid.find( coordsToStrig( (droid_y + 1), droid_x ) ) == known_grid.end() )
    return NORTH;
  if( known_grid.find( coordsToStrig( (droid_y - 1), droid_x ) ) == known_grid.end() )
    return SOUTH;
  if( known_grid.find( coordsToStrig( droid_y, (droid_x - 1) ) ) == known_grid.end() )
    return WEST;
  if( known_grid.find( coordsToStrig( droid_y, (droid_x + 1) ) ) == known_grid.end() )
    return EAST;
  return NONE;
}

string coordsToStrig( const int& y, const int& x ) {
  return to_string( y ) + ":"  + to_string( x );
}

pair<int, int> getPositionChange( const int& step ) {
  int y_change = 0, x_change = 0;
  if( step == NORTH ) 
    y_change = 1;
  if( step == SOUTH )
    y_change = -1;
  if( step == WEST )
    x_change = -1;
  if( step == EAST )
    x_change = 1;
  return make_pair( y_change, x_change );
}

int getOppositeStep( const int& step ) {
  if( step == NORTH )
    return SOUTH;
  if( step == SOUTH )
    return NORTH;
  if( step == WEST )
    return EAST;
  if( step == EAST )
    return WEST;
  return NONE;
}

void updateGridDimensions( int& min_y, int& max_y, int& min_x, int& max_x, const int& droid_y, const int& droid_x ) {
  min_y = droid_y < min_y ? droid_y : min_y;
  max_y = droid_y > max_y ? droid_y : max_y;
  min_x = droid_x < min_x ? droid_x : min_x;
  max_x = droid_x > max_x ? droid_x : max_x;
}

void printGrid( unordered_map<string, char>& known_grid, const int& min_y, const int& max_y, const int& min_x, const int& max_x, const string& droid ) {
  string id;
  for( int y = max_y; y >= min_y; --y ) {
    for( int x = min_x; x <= max_x; ++x ) {
      id = coordsToStrig( y, x );
      if( known_grid.find( id ) != known_grid.end() ) {
        if( id == droid )
          cout << "D";
        else
          cout << known_grid[id];
      }
      else 
        cout << '?';
    }
    cout << endl;
  }
}

pair<int, int> breadthFirstSearch( unordered_map<string, char>& know_grid ) {
  unordered_set<string> visited;
  deque<BFSNode> to_visit;
  BFSNode tmp( 0, 0, 0, "0:0" );
  to_visit.push_back( tmp );
  while( true ) {
    tmp = to_visit.front();
    to_visit.pop_front();
    visited.insert( tmp.pos_str );
    if( know_grid[tmp.pos_str] == '*' ) {
      cout << "(fewest) number of steps to oxygen system: " << tmp.distance << endl;
      return make_pair( tmp.y_pos, tmp.x_pos );
    }
    else {
      string next_pos = coordsToStrig( (tmp.y_pos + 1), tmp.x_pos );
      if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
        to_visit.push_back( BFSNode( (tmp.y_pos + 1), tmp.x_pos, (tmp.distance + 1), next_pos ) );
      
      next_pos = coordsToStrig( (tmp.y_pos - 1), tmp.x_pos );
      if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
        to_visit.push_back( BFSNode( (tmp.y_pos - 1), tmp.x_pos, (tmp.distance + 1), next_pos ) );
      
      next_pos = coordsToStrig( tmp.y_pos, (tmp.x_pos + 1) );
      if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
        to_visit.push_back( BFSNode( tmp.y_pos, (tmp.x_pos + 1), (tmp.distance + 1), next_pos ) );
      
      next_pos = coordsToStrig( tmp.y_pos, (tmp.x_pos - 1) );
      if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
        to_visit.push_back( BFSNode( tmp.y_pos, (tmp.x_pos - 1), (tmp.distance + 1), next_pos ) );
    }
  }
  return make_pair( tmp.y_pos, tmp.x_pos );
}

int fillGridWithOxigen( unordered_map<string, char>& know_grid, const int& y_pos, const int& x_pos ) {
  unordered_set<string> visited;
  deque<BFSNode> to_visit;
  BFSNode tmp( y_pos, x_pos, 0, coordsToStrig( y_pos, x_pos ) );
  to_visit.push_back( tmp ); 
  while( to_visit.size() > 0 ){
    tmp = to_visit.front();
    to_visit.pop_front();
    visited.insert( tmp.pos_str );

    string next_pos = coordsToStrig( (tmp.y_pos + 1), tmp.x_pos );
    if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
      to_visit.push_back( BFSNode( (tmp.y_pos + 1), tmp.x_pos, (tmp.distance + 1), next_pos ) );
      
    next_pos = coordsToStrig( (tmp.y_pos - 1), tmp.x_pos );
    if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
      to_visit.push_back( BFSNode( (tmp.y_pos - 1), tmp.x_pos, (tmp.distance + 1), next_pos ) );
      
    next_pos = coordsToStrig( tmp.y_pos, (tmp.x_pos + 1) );
    if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
      to_visit.push_back( BFSNode( tmp.y_pos, (tmp.x_pos + 1), (tmp.distance + 1), next_pos ) );
      
    next_pos = coordsToStrig( tmp.y_pos, (tmp.x_pos - 1) );
    if( know_grid[next_pos] != '#' && visited.count( next_pos ) == 0 )
      to_visit.push_back( BFSNode( tmp.y_pos, (tmp.x_pos - 1), (tmp.distance + 1), next_pos ) );
  }
  return tmp.distance;
}