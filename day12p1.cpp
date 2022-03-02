#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
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

  void applyVelocity( void ) {
    x_pos += x_velocity;
    y_pos += y_velocity;
    z_pos += z_velocity;
  }

  int getTotalEnergy( void ) {
    int p_energy = abs( x_pos ) + abs( y_pos ) + abs( z_pos );
    int k_energy = abs( x_velocity ) + abs( y_velocity ) + abs( z_velocity );
    return p_energy * k_energy;
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
void simuateKSteps( const int&, vector<Moon>& );
int getTotalSystemEnergy( vector<Moon>& );

int main( void ) {
  vector<Moon> moons = getMoonList();
  simuateKSteps( 1000, moons );
  cout << getTotalSystemEnergy( moons ) << endl; 
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

void simuateKSteps( const int& k, vector<Moon>& moons ) {
  for( int step = 0; step < k; ++step ) {
    for( unsigned int m1 = 0; m1 < moons.size(); ++m1 ) {
      for( unsigned int m2 = m1 + 1; m2 < moons.size(); ++m2 ) {
        if( moons[m1].getAxisPosition( "x" ) < moons[m2].getAxisPosition( "x" ) ) {
            moons[m1].increaseAxisVelocity( "x" );
            moons[m2].decreaseAxisVelocity( "x" );
        }
        if( moons[m1].getAxisPosition( "x" ) > moons[m2].getAxisPosition( "x" ) ) {
            moons[m1].decreaseAxisVelocity( "x" );
            moons[m2].increaseAxisVelocity( "x" );
        }
        if( moons[m1].getAxisPosition( "y" ) < moons[m2].getAxisPosition( "y" ) ) {
            moons[m1].increaseAxisVelocity( "y" );
            moons[m2].decreaseAxisVelocity( "y" );
        }
        if( moons[m1].getAxisPosition( "y" ) > moons[m2].getAxisPosition( "y" ) ) {
            moons[m1].decreaseAxisVelocity( "y" );
            moons[m2].increaseAxisVelocity( "y" );
        }
        if( moons[m1].getAxisPosition( "z" ) < moons[m2].getAxisPosition( "z" ) ) {
            moons[m1].increaseAxisVelocity( "z" );
            moons[m2].decreaseAxisVelocity( "z" );
        }
        if( moons[m1].getAxisPosition( "z" ) > moons[m2].getAxisPosition( "z" ) ) {
            moons[m1].decreaseAxisVelocity( "z" );
            moons[m2].increaseAxisVelocity( "z" );
        }
      }
    }
    for( unsigned int m1 = 0; m1 < moons.size(); ++m1 ) {
      moons[m1].applyVelocity();
    }
  }
}

int getTotalSystemEnergy( vector<Moon>& moons ) {
  int total_energy = 0;
  for( unsigned int m = 0; m < moons.size(); ++m ) {
    total_energy += moons[m].getTotalEnergy();
  }
  return total_energy;
}