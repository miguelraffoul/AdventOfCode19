#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

vector< vector<string> > getWireTopology( void );
int getClosestIntersection( const vector< vector<string> >& );
pair<char, int> parseInstruction( string  );
string coordToString( const int&, const int& );
int getManhattanDistance( const int&, const int& );

int main( void ) {
  vector< vector<string> > wires = getWireTopology();
  cout << getClosestIntersection( wires ) << endl;
  return EXIT_SUCCESS;
}

vector< vector<string> > getWireTopology( void ) {
  vector< vector<string> > wires;
  string input;
  while( cin >> input ) {
    for( unsigned int x = 0; x < input.size(); ++x ) {
      if( input[x] == ',' )
        input[x] = ' ';
    }
    istringstream iss( input );
    vector<string> wire;
    string instruction;
    while( iss >> instruction ) {
      wire.push_back( instruction );
    }
    wires.push_back( wire );
  }
  return wires;
}

int getClosestIntersection( const vector< vector<string> >& wires ) {
  unordered_map<string, int> coordinates;
  int x_coord, y_coord;
  int closest_distance = 0x7fffffff;
  for( unsigned int w = 0; w < wires.size(); ++w ) {
    x_coord = y_coord = 0;
    pair<char, int> instruction;
    for( unsigned int i = 0; i < wires[w].size(); ++i ) {
      instruction = parseInstruction( wires[w][i] );
      while( instruction.second-- ) {
        if( instruction.first == 'U' )
          ++y_coord;
        else if( instruction.first == 'R' )
          ++x_coord;
        else if( instruction.first == 'D' )
          --y_coord;
        else //L
          --x_coord;
        pair<unordered_map<string,int>::iterator, bool> result;
        result = coordinates.insert( make_pair( coordToString( x_coord, y_coord ), w ) );
        if( !result.second  && result.first->second != w ) {
          int tmp_distance = getManhattanDistance( x_coord, y_coord );
          if(  tmp_distance < closest_distance )
            closest_distance = tmp_distance;
        }
      }
    }
  }
  return closest_distance;
}

pair<char, int> parseInstruction( string instruction ) {
  char direction = instruction[0];
  instruction[0] = ' ';
  istringstream iss( instruction );
  int moves;
  iss >> moves;
  return make_pair( direction, moves );
}

string coordToString( const int& x, const int& y ) {
  ostringstream oss;
  oss << x << "," << y;
  return oss.str();
}

int getManhattanDistance( const int& x, const int& y ) {
  int abs_x = x < 0 ? (x * -1) : x;
  int abs_y = y < 0 ? (y * -1) : y;
  return (abs_x + abs_y);
}