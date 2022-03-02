#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> parseInputSignal( const string& );
void printSignal( const vector<int>& );

int main( void ) {
  string input;
  cin >> input;

  vector<int> input_signal = parseInputSignal( input );
  vector<int> output_signal = input_signal;
  int pattern[] = {0, 1, 0, -1};
  for( unsigned int phase = 0; phase < 100; ++phase ) {
    for( unsigned int os = 0; os < output_signal.size(); ++os ) {
      int sum = 0;
      int idx = 0;
      int counter = os + 1;
      for( unsigned int is = 0; is < input_signal.size(); ++is ) {
        if( is == 0 )
          --counter;
        if( counter == 0 ) {
          ++idx; 
          counter = os + 1;
        }
        if( idx > 3 )
          idx = 0;
        sum += input_signal[is] * pattern[idx];
        --counter;
      }
      output_signal[os] = (sum < 0 ? (sum * -1) : sum) % 10;
    }
    input_signal = output_signal;
  }
  printSignal( input_signal );

  return EXIT_SUCCESS;
}

vector<int> parseInputSignal( const string& input ) {
  vector<int> signal;
  for( unsigned int c = 0; c < input.size(); ++c ) {
    signal.push_back( input[c] - '0' );
  }
  return signal;
}

void printSignal( const vector<int>& signal ) {
  for( unsigned int i = 0; i < signal.size(); ++i ) {
    cout << signal[i];
  }
  cout << endl;
}
