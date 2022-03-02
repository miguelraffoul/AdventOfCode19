#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

vector< vector<string> > getWireTopology( void );
int getDistanceToFirstIntersect( const vector< vector<string> >& );
pair<char, int> parseInstruction( string  );
string coordToString( const int&, const int& );

int main( void ) {
  vector< vector<string> > wires = getWireTopology();
  cout << getDistanceToFirstIntersect( wires ) << endl;
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

int getDistanceToFirstIntersect( const vector< vector<string> >& wires ) {
  unordered_map< string, pair<int, int> > points;
  int x_coord, y_coord;
  int total_distance = 0x7fffffff;
  for( unsigned int w = 0; w < wires.size(); ++w ) {
    x_coord = y_coord = 0;
    pair<char, int> instruction;
    int distance = 0;
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
        ++distance;
        pair<unordered_map< string, pair<int,int> >::iterator, bool> result;
        result = points.insert( make_pair( coordToString( x_coord, y_coord ), make_pair( w, distance ) ) );
        if( !result.second ) {
          if( result.first->second.first != w ) {
            if( (result.first->second.second + distance) < total_distance )
              total_distance = result.first->second.second + distance;
          }
        }
      }
    }
  }
  return total_distance;
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