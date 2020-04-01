#include "butterworth_lowpass_filter.h"

#include <iostream>

int main(int argc, char** argv) {
  ButterworthLowpassFilter filter;
  filter.SetFilterWp(45);
  filter.SetFilterWs(75);
  filter.SetFilterRp(3);
  filter.SetFilterRs(10);
  filter.Process();
  std::cout << "filter order: " << filter.GetFilterOrderAsDouble()
            << " => " << filter.GetFilterOrderAsInt()
            << std::endl;
  std::cout << "filter cutoff: " << filter.GetFilterCutoffFrequency() << std::endl;
}
