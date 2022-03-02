#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<short> parseInputSignal( const string&, const unsigned int& );
void printSignalSection( const vector<short>&, const unsigned int&, const unsigned int& );

int main( void ) {
  string input;
  cin >> input;

  vector<short> input_signal = parseInputSignal( input, 10000 );
  unsigned int target = 5976683;
  for( unsigned int phase = 0; phase < 100; ++phase ) {
    int sum = 0;
    for( unsigned int d = input_signal.size() - 1; d >= (target - 1); --d ) {
      sum += input_signal[d];
      input_signal[d] = sum % 10;
    }
  }
  printSignalSection( input_signal, target, 8 );

  return EXIT_SUCCESS;
}

vector<short> parseInputSignal( const string& input, const unsigned int& copies ) {
  vector<short> signal;
  for( unsigned int copy = 0; copy < copies; ++copy ) {
    for( unsigned int digit = 0; digit < input.size(); ++digit ) {
      signal.push_back( input[digit] - '0' );
    }
  }
  return signal;
}

void printSignalSection( const vector<short>& signal, const unsigned int& pos, const unsigned int& digits ) {
  for( unsigned int i = pos; (i < signal.size() && i < (pos + digits)); ++i ) {
    cout << signal[i];
  }
  cout << endl;
}
