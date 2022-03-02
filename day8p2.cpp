#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> getDecodecPicture( const string&, const int&, const int& );
void printDecodedPicture( const vector<string>& );

int main( void ) {
  int width = 25, height = 6;
  string pixels;
  cin >> pixels;
  printDecodedPicture( getDecodecPicture( pixels, width, height ) );
  return EXIT_SUCCESS;
}

vector<string> getDecodecPicture( const string& pixels, const int& width, const int& height ) {
  vector<string> picture;
  string row;
  int counter = 0;
  for( int p = 0; p < (width * height); ++p ) {
    ++counter;
    int pixel = p;
    while( pixels[pixel] == '2' ) {
      pixel += (width * height);
    }
    char c = pixels[pixel] == '0' ? ' ' : '#';
    row.push_back( c );
    if( counter == width ) {
      picture.push_back( row );
      row.clear();
      counter = 0;
    }
  }
  return picture;
}

void printDecodedPicture( const vector<string>& picture ) {
  for( unsigned int i = 0; i < picture.size(); ++i ) {
    cout << picture[i] << endl;
  }
}