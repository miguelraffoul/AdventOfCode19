#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Planet {
  string name;
  vector<Planet*> child_planets;

  Planet( const string& _name ) {
    name = _name;
  }
};

Planet* buildTree( void );
pair<string, string> splitInput( string& );
int getTotalOrbits( Planet*, int );

int main( void ) {
  Planet* root = buildTree();
  cout << getTotalOrbits( root, 0 ) << endl;
  return EXIT_SUCCESS;
}

Planet* buildTree( void ) {
  unordered_map<string, Planet*> addresses;
  pair<string, string> planet_pair;
  string input;
  while( cin >> input ) {
    planet_pair = splitInput( input );
    if( addresses.find( planet_pair.first ) == addresses.end() ) 
      addresses.insert( make_pair( planet_pair.first, new Planet( planet_pair.first ) ) );
    if( addresses.find( planet_pair.second ) == addresses.end() ) 
      addresses.insert( make_pair( planet_pair.second, new Planet( planet_pair.second ) ) );
    addresses[planet_pair.first]->child_planets.push_back( addresses[planet_pair.second] );   
  }
  return addresses["COM"];
}

pair<string, string> splitInput( string& input ) {
  input[3] = ' ';
  istringstream iss( input );
  string p1, p2;
  iss >> p1;
  iss >> p2;
  return make_pair( p1, p2 );
}

int getTotalOrbits( Planet* planet, int depth ) {
  int total_orbits = depth;
  //cout << planet->name << endl;
  for( int x = 0; x < planet->child_planets.size(); ++x ) {
    total_orbits += getTotalOrbits( planet->child_planets[x], depth + 1 );
  }
  return total_orbits;
}