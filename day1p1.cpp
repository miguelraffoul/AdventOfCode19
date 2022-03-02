#include <cstdlib>
#include <iostream>

using namespace std;

int main( void ) {
  int total_fuel = 0;
  int input_mass;
  while( cin >> input_mass ) {
    total_fuel += (input_mass / 3) - 2;
  }
  cout << total_fuel << endl;
  return EXIT_SUCCESS;
}