#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main( void ) {
  string input;
  cin >> input;

  int layer_size = 25 * 6;
  int min_pixel_zero_count = layer_size * 2;
  int pixel_one_count, pixel_two_count;
  int pzc, poc, ptc;
  int counter = 0;
  for( unsigned int p = 0; p < input.size(); ++p ) {
    ++counter;
    if( input[p] == '0' )
      pzc += 1;
    else if( input[p] == '1' )
      poc += 1;
    else if( input[p] == '2' )
      ptc += 1;
    else
      cout << input[p] << endl;
    
    if( counter == 150 ) {
      if( pzc < min_pixel_zero_count ) {
        min_pixel_zero_count = pzc;
        pixel_one_count = poc;
        pixel_two_count = ptc;
      }
      pzc = poc = ptc = 0;
      counter = 0;
    }
  }
  cout << (pixel_one_count * pixel_two_count) << endl;
  return EXIT_SUCCESS;
}