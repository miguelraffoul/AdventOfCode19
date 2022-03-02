#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<string> readInputGrid( void );
vector< pair<int, int> > getAsteroidList( const vector<string>& );
string idxPairToStr( const unsigned int&, const unsigned int& );
bool horizontalIsAsteroidVisible( const vector<string>&, const pair<int, int>&, const pair<int, int>& );
bool verticalIsAsteroidVisible( const vector<string>&, const pair<int, int>&, const pair<int, int>& );
bool slopeIsAsteroidVisible( const vector<string>&, const pair<int, int>&, const pair<int, int>& );

int main( void ) {
  vector<string> grid = readInputGrid();
  vector< pair<int, int> > asteroids = getAsteroidList( grid );
  unordered_map<string, bool> visible_asteroid_pair;

  int visible_count = 0, tmp_count, temp_x, temp_y;
  bool is_visible;
  for( unsigned int i = 0; i < asteroids.size(); ++i ) {
    tmp_count = 0;
    for( unsigned int j = 0; j < asteroids.size(); ++j ) {
      if( j != i ) {
        if( visible_asteroid_pair.find( idxPairToStr( i, j ) ) != visible_asteroid_pair.end() ) {
          is_visible = visible_asteroid_pair[idxPairToStr( i, j )];
        }
        else if( asteroids[i].first == asteroids[j].first ) {
          if( asteroids[i].second < asteroids[j].second ) 
            is_visible = horizontalIsAsteroidVisible( grid, asteroids[i], asteroids[j] );
          else
            is_visible = horizontalIsAsteroidVisible( grid, asteroids[j], asteroids[i] );
          visible_asteroid_pair[idxPairToStr( i, j )] = is_visible;
          visible_asteroid_pair[idxPairToStr( j, i )] = is_visible;
        }
        else if( asteroids[i].second == asteroids[j].second ) {
          if( asteroids[i].first < asteroids[j].first ) 
            is_visible = verticalIsAsteroidVisible( grid, asteroids[i], asteroids[j] );
          else
            is_visible = verticalIsAsteroidVisible( grid, asteroids[j], asteroids[i] );
          visible_asteroid_pair[idxPairToStr( i, j )] = is_visible;
          visible_asteroid_pair[idxPairToStr( j, i )] = is_visible;
        }
        else {
          if( asteroids[i].second < asteroids[j].second ) 
            is_visible = slopeIsAsteroidVisible( grid, asteroids[i], asteroids[j] );
          else
            is_visible = slopeIsAsteroidVisible( grid, asteroids[j], asteroids[i] );
          visible_asteroid_pair[idxPairToStr( i, j )] = is_visible;
          visible_asteroid_pair[idxPairToStr( j, i )] = is_visible;
        }
        tmp_count += is_visible ? 1 : 0;
      }
    }
    if( visible_count < tmp_count ) {
      visible_count = tmp_count;
      temp_y = asteroids[i].first;
      temp_x = asteroids[i].second;  
    }
    //visible_count = visible_count < tmp_count ? tmp_count : visible_count;
  }
  cout << "Asteroid in position x: " << temp_x << ", y:" << temp_y;
  cout << " can see " << visible_count << " asteroids." << endl;
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

vector< pair<int, int> > getAsteroidList( const vector<string>& grid ) {
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

string idxPairToStr( const unsigned int& i1, const unsigned int& i2 ) {
  return to_string(i1) + ":" + to_string(i2);
}

bool horizontalIsAsteroidVisible( const vector<string>& grid, const pair<int, int>& p1, const pair<int, int>& p2 ) {
  for( int x = p1.second + 1; x < p2.second; ++ x ) {
    if( grid[p1.first][x] == '#' )
      return false;
  } 
  return true;
}

bool verticalIsAsteroidVisible( const vector<string>& grid, const pair<int, int>& p1, const pair<int, int>& p2 ) {
  for( int y = p1.first + 1; y < p2.first; ++y ) {
    if( grid[y][p1.second] == '#' )
      return false;
  }
  return true;
}

bool slopeIsAsteroidVisible( const vector<string>& grid, const pair<int, int>& p1, const pair<int, int>& p2 ) {
  double dx = p2.second - p1.second;
  double dy = p2.first - p1.first;
  for( int x = p1.second + 1; x < p2.second; ++x ) {
    double y = p1.first + ((dy * (x - p1.second)) / dx);
    if( fmod( y, 1 ) == 0 && grid[(int) y][x] == '#' ) {
      return false;
    }
  }
  return true;
}