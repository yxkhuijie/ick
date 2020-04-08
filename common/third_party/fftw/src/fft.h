#include <string>

class FFT {
 public:
  FFT();
  ~FFT();

 public:
  bool calculate();
  void setData(double* data, int length);

  std::string getResultAsString(int index); 
  int getResultDataLength();

  void clear();
 
 private:
  double* data_;
  int data_length_; 
  int data_dim_;

  double* result_real_;
  double* result_imaginary_;
  int result_data_length_;

  bool parameter_changed_;
};
