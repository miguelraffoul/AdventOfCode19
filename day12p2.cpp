#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

class Moon {
  public:
  Moon( const int& _x_pos, const int& _y_pos, const int& _z_pos ) :
    x_pos( _x_pos ),
    y_pos( _y_pos ),
    z_pos( _z_pos ),
    x_velocity( 0 ),
    y_velocity( 0 ),
    z_velocity( 0 )
  {
  }

  void increaseAxisVelocity( const string& axis ) {
    if( axis == "x" )
      ++x_velocity;
    else if( axis == "y" )
      ++y_velocity;
    else
      ++z_velocity;
  }

  void decreaseAxisVelocity( const string& axis ) {
    if( axis == "x" )
      --x_velocity;
    else if( axis == "y" )
      --y_velocity;
    else
      --z_velocity;
  }

  int getAxisPosition( const string& axis ) {
    if( axis == "x" )
      return x_pos;
    else if( axis == "y" )
      return y_pos;
    else
      return z_pos;
  }

  int getAxisVelocity( const string& axis ) {
    if( axis == "x" )
      return x_velocity;
    else if( axis == "y" )
      return y_velocity;
    else
      return z_velocity;
  }

  void applyVelocity( void ) {
    x_pos += x_velocity;
    y_pos += y_velocity;
    z_pos += z_velocity;
  }

  string getMoonAxisState( const string& axis ) {
    return to_string( getAxisPosition( axis ) ) + ":" + to_string( getAxisVelocity( axis ) ) + "|";
  }

  private:
  int x_pos;
  int y_pos;
  int z_pos;
  int x_velocity;
  int y_velocity;
  int z_velocity; 
};

vector<Moon> getMoonList( void );
long long int getAxisLoopSteps( vector<Moon>&, const string& );
void computeMaxPrimeFactors( long long int&, unordered_map<int, int>& );

int main( void ) {
  vector<Moon> moons = getMoonList();
  vector<long long int> steps;
  
  steps.push_back( getAxisLoopSteps( moons, "x" ) );
  steps.push_back( getAxisLoopSteps( moons, "y" ) );
  steps.push_back( getAxisLoopSteps( moons, "z" ) );
  cout << "X axis took " << steps[0] << " to a previous state" << endl;
  cout << "Y axis took " << steps[1] << " to a previous state" << endl;
  cout << "Z axis took " << steps[2] << " to a previous state" << endl;

  unordered_map<int, int> prime_factors;
  unordered_map<int, int>::iterator it;
  long long int lcm = 1;
  computeMaxPrimeFactors( steps[0], prime_factors );
  computeMaxPrimeFactors( steps[1], prime_factors );
  computeMaxPrimeFactors( steps[2], prime_factors );
  for( it = prime_factors.begin(); it != prime_factors.end(); ++it ) {
    lcm *= pow( it->first, it->second );
  }
  cout << "LCM is: " << lcm << endl;
  return EXIT_SUCCESS;
}

vector<Moon> getMoonList( void ) {
  vector<Moon> moon_list;
  string input;
  int x, y, z;
  while( getline( cin, input ) ){
    for( unsigned int c = 0; c < input.size(); ++c ) {
      if( (input[c] < '0' || input[c] > '9') && input[c] != '-' )
        input[c] = ' ';
    }
    istringstream iss( input );
    iss >> x;
    iss >> y;
    iss >> z;
    Moon new_moon( x, y, z );
    moon_list.push_back( new_moon );
  }
  return moon_list;
}

long long int getAxisLoopSteps( vector<Moon>& moons, const string& axis ) {
  unordered_set<string> system_states;
  string system_state;
  long long int k = 0;
  for( unsigned int m1 = 0; m1 < moons.size(); ++m1 ) {
    system_state += moons[m1].getMoonAxisState( axis );
  }
  system_states.insert( system_state );
  while( true ) {
    for( unsigned int m1 = 0; m1 < moons.size(); ++m1 ) {
      for( unsigned int m2 = m1 + 1; m2 < moons.size(); ++m2 ) {
        if( moons[m1].getAxisPosition( axis ) < moons[m2].getAxisPosition( axis ) ) {
            moons[m1].increaseAxisVelocity( axis );
            moons[m2].decreaseAxisVelocity( axis );
        }
        if( moons[m1].getAxisPosition( axis ) > moons[m2].getAxisPosition( axis ) ) {
            moons[m1].decreaseAxisVelocity( axis );
            moons[m2].increaseAxisVelocity( axis );
        }
      }
    }
    system_state.clear();
    for( unsigned int m1 = 0; m1 < moons.size(); ++m1 ) {
      moons[m1].applyVelocity();
      system_state += moons[m1].getMoonAxisState( axis );
    }
    ++k;
    if( system_states.find( system_state ) != system_states.end() ) 
      break;
    system_states.insert( system_state );
  }
  return k;
}

void computeMaxPrimeFactors( long long int& steps, unordered_map<int, int>& pf ) {
  int base = 2;
  int exp = 0;
  while( steps % base == 0 ) {
    steps /= base;
    ++exp;
  }
  if( pf.find( base ) != pf.end() ) {
    pf[base] = exp > pf[base] ? exp: pf[base];
  }
  else {
    pf[base] = exp;
  }
  for( unsigned int b = 3; b < sqrt( steps ); b += 2 ) {
    base = b;
    exp = 0;
    while( steps % base == 0 ) {
      steps /= base;
      ++exp;
    }
    if( pf.find( base ) != pf.end() ) {
      pf[base] = exp > pf[base] ? exp: pf[base];
    }
    else {
      pf[base] = exp;
    }
  }
  if( steps > 2 ) {
    pf[steps] = 1;
  }
}