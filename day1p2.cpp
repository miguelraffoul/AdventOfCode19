#include <cstdlib>
#include <iostream>

using namespace std;

int main( void ) {
  int input_mass, module_fuel, total_fuel = 0;
  while( cin >> input_mass ) {
    module_fuel = (input_mass / 3) - 2;
    while( module_fuel > 0 ) {
      total_fuel += module_fuel;
      module_fuel = (module_fuel / 3) - 2;
    }
  }
  cout << total_fuel << endl;
  return EXIT_SUCCESS;
}