#include "third_party/fftw/src/fft.h"

#include <iostream>

int main(int argc, char** argv) {
  double test_data1[] = { 1, 2, 3, 4 };
  FFT fft;
  fft.setData(test_data1, 4);
  fft.calculate();
  int result_length = fft.getResultDataLength();
  for (int i = 0; i < result_length; ++i) {
    std::cout << fft.getResultAsString(i) << std::endl;
  }

  double test_data2[] = { 1,2,3,4,5 };
  fft.setData(test_data2, 5);
  fft.calculate();
  result_length = fft.getResultDataLength();
  for (int i = 0; i < result_length; ++i) {
    std::cout << fft.getResultAsString(i) << std::endl;
  }
}

