#include "third_party/fftw/src/fft.h"

#include <sstream>

#include "third_party/fftw/fftw-3.3.8/api/fftw3.h"

FFT::FFT()
    : data_(nullptr)
    , data_length_(0)
    , data_dim_(1)
    , result_real_(nullptr)
    , result_imaginary_(nullptr)
    , result_data_length_(0)
    , parameter_changed_(false) {}

FFT::~FFT() {
  if (result_real_ != nullptr) {
    delete[] result_real_;
  }
  if (result_imaginary_ != nullptr) {
    delete[] result_imaginary_;
  }
}

bool FFT::calculate() {
  if (data_ == nullptr || data_length_ <= 0) return false;
  result_data_length_ = data_length_ / 2 + 1;
  result_real_ = new double[result_data_length_];
  result_imaginary_ = new double[result_data_length_];
  
  fftw_complex* result =
      (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * result_data_length_);
  fftw_plan plan = fftw_plan_dft_r2c_1d(
      data_length_, data_, result, FFTW_ESTIMATE);
  fftw_execute(plan);

  fftw_destroy_plan(plan);
  for (int i = 0; i < result_data_length_; ++i) {
    result_real_[i] = result[i][0];
    result_imaginary_[i] = result[i][1];
  }
  return true;
}

void FFT::setData(double* data, int length) {
  this->data_ = data;
  this->data_length_ = length;
  this->clear();
}

std::string FFT::getResultAsString(int index) {
  if (result_real_ == nullptr || result_imaginary_ == nullptr ||
      index >= result_data_length_) return ""; 
  std::stringstream ss;
  ss << result_real_[index]
     << " "
     << result_imaginary_[index]
     << "i";
  return ss.str();
}

int FFT::getResultDataLength() {
  return result_data_length_;
}

void FFT::clear() {
  if (result_real_ != nullptr) {
    delete[] result_real_;
    result_real_ = nullptr;
  }
  if (result_imaginary_ != nullptr) {
    delete[] result_imaginary_;
    result_imaginary_ = nullptr;
  }
}
