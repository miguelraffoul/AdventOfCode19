#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

#define PI 3.1415926535897932384626433832795028841971

using namespace std;

vector<string> readInputGrid( void );
vector<pair<int, int>> getAsteroidList( const vector<string>& );
map<double, map<double, pair<int,int>>> getArrangedAsteroids( const vector<pair<int, int>>&, const pair<int, int>& );
double getPositiveAngle( const double&, const double& );
void printArrangedAsteroids( const map<double, map<double, pair<int,int>>>& );
int getKDeletedAsteroid( const int&, map<double, map<double, pair<int,int>>>& );

int main( void ) {
  cout << std::fixed;
  cout << std::setprecision(50);
  vector<string> grid = readInputGrid();
  vector<pair<int, int>> asteroids = getAsteroidList( grid );
  map<double, map<double, pair<int,int>>> arranged_asteroids = getArrangedAsteroids( asteroids, make_pair( 21, 20 ) );
  //printArrangedAsteroids( arranged_asteroids );
  cout << "==============================================================" << endl;
  cout << getKDeletedAsteroid( 200, arranged_asteroids ) << endl;
  cout << "==============================================================" << endl;
  //printArrangedAsteroids( arranged_asteroids );
  return EXIT_SUCCESS;
}

vector<string> readInputGrid( void ) {
  vector<string> grid;
  string row;
  while( cin >> row ) {
    grid.push_back( row );
  }
  return grid;
}

vector<pair<int, int>> getAsteroidList( const vector<string>& grid ) {
  vector< pair<int, int> > asteroids;
  for( unsigned int y = 0; y < grid.size(); ++y ) {
    for( unsigned int x = 0; x < grid[y].size(); ++x ) {
      if( grid[y][x] == '#' ) {
        asteroids.push_back( make_pair( y, x ) );
      }
    }
  }
  return asteroids;
}

map<double, map<double, pair<int,int>>> getArrangedAsteroids( const vector<pair<int, int>>& asteroids, const pair<int, int>& reference ) {
  map<double, map<double, pair<int,int>>> arranged_asteroids;
  int tmp_diff_y, tmp_diff_x;
  double tmp_angle, tmp_distance;
  for( unsigned int a = 0; a < asteroids.size(); ++a ) {
    if( asteroids[a].first != reference.first || asteroids[a].second != reference.second ) {
      tmp_diff_y = asteroids[a].first - reference.first;
      tmp_diff_x = asteroids[a].second - reference.second;
      tmp_angle = getPositiveAngle( (tmp_diff_y * -1), tmp_diff_x ); //-1 multiplication to account for inverted y axis
      tmp_distance = sqrt( (tmp_diff_y * tmp_diff_y) + (tmp_diff_x * tmp_diff_x) );
      if( arranged_asteroids.find( tmp_angle ) == arranged_asteroids.end() ) {
        map<double, pair<int,int>> tmp_map;
        tmp_map.insert( make_pair( tmp_distance, asteroids[a] ) );
        arranged_asteroids.insert( make_pair( tmp_angle, tmp_map ) );
      }
      else {
        arranged_asteroids[tmp_angle].insert( make_pair( tmp_distance, asteroids[a] ) );
      }
    }
  }
  return arranged_asteroids;
}

double getPositiveAngle( const double& y, const double& x ) {
  double angle = atan2( y, x ) * 180 / PI;
  return angle >= 0 ? angle : (360 + angle);
}

void printArrangedAsteroids( const map<double, map<double, pair<int,int>>>& arranged_asteroids ) {
  map<double, map<double, pair<int,int>>>::const_iterator cit;
  for( cit = arranged_asteroids.cbegin(); cit != arranged_asteroids.cend(); ++cit ) {
    map<double, pair<int,int>>::const_iterator tmp_cit;
    cout << cit->first << " -> ";
    for( tmp_cit = cit->second.cbegin(); tmp_cit != cit->second.cend(); ++tmp_cit ) {
      cout << tmp_cit->first << ": (" << tmp_cit->second.first << ", " << tmp_cit->second.second << ")    ";
    }
    cout << endl;
  }
}

int getKDeletedAsteroid( const int& k, map<double, map<double, pair<int,int>>>& arranged_asteroids ) {
  map<double, map<double, pair<int,int>>>::iterator it = arranged_asteroids.find( 90 );
  pair<int,int> tmp_asteroid;
  int deleted_asteroids = 0;
  do {
    if( it->second.size() > 0 ) {
      tmp_asteroid = it->second.begin()->second;
      it->second.erase( it->second.begin() );
      ++deleted_asteroids;
    }
    if( it == arranged_asteroids.begin() ) {
      it = arranged_asteroids.end();
    }
    --it;
  } while( deleted_asteroids < k );
  cout << k <<": (" << tmp_asteroid.first << ", " << tmp_asteroid.second << "):  ";
  return (tmp_asteroid.second * 100 +  tmp_asteroid.first);
}