#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Planet {
  string name;
  Planet* parent_planet;

  Planet( const string& _name ) {
    name = _name;
    parent_planet = NULL;
  }
};

unordered_map<string, Planet*> buildTree( void );
pair<string, string> splitInput( string& );
int getDistanceToSanta( unordered_map<string, Planet*>& );

int main( void ) {
  unordered_map<string, Planet*> addresses = buildTree();
  cout << getDistanceToSanta( addresses ) << endl;
  return EXIT_SUCCESS;
}

unordered_map<string, Planet*> buildTree( void ) {
  unordered_map<string, Planet*> addresses;
  pair<string, string> planet_pair;
  string input;
  while( cin >> input ) {
    planet_pair = splitInput( input );
    if( addresses.find( planet_pair.first ) == addresses.end() ) 
      addresses.insert( make_pair( planet_pair.first, new Planet( planet_pair.first ) ) );
    if( addresses.find( planet_pair.second ) == addresses.end() ) 
      addresses.insert( make_pair( planet_pair.second, new Planet( planet_pair.second ) ) );
    addresses[planet_pair.second]->parent_planet = addresses[planet_pair.first];
  }
  return addresses;
}

pair<string, string> splitInput( string& input ) {
  input[3] = ' ';
  istringstream iss( input );
  string p1, p2;
  iss >> p1;
  iss >> p2;
  return make_pair( p1, p2 );
}

int getDistanceToSanta( unordered_map<string, Planet*>& addresses ) {
  unordered_map<string, int> path;
  Planet* p1 = addresses["YOU"]->parent_planet;
  Planet* p2 = addresses["SAN"]->parent_planet;
  int d1 = 0, d2 = 0;
  while( true ) {
    if( p1 != NULL ) {
      if( path.find( p1->name ) != path.end() )
        return (d1 + path[p1->name]);
      path[p1->name] = d1;
      p1 = p1->parent_planet;
      ++d1;
    }
    if( p2 != NULL ) {
      if( path.find( p2->name ) != path.end() )
        return (d2 + path[p2->name]);
      path[p2->name] = d2;
      p2 = p2->parent_planet;
      ++d2;
    }
  }
  return 0;
}