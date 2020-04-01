#include "butterworth_lowpass_filter.h"

#include <math.h>
#include <cmath>

#ifndef PI
#define PI 3.1415926
#endif

ButterworthLowpassFilter::ButterworthLowpassFilter()
    : filter_wp_(0.0f)
    , filter_ws_(0.0f)
    , filter_rp_(0.0f)
    , filter_rs_(0.0f)
    , filter_order_(0.0f) {}

ButterworthLowpassFilter::~ButterworthLowpassFilter() {}

/*
 *           10^(0.1 * Rs) - 1
 *       lg(--------------------)
 *           10^(0.1 * Rp) - 1
 * N >= -------------------------------
 *                    Ws
 *            2 * lg(----)
 *                    Wp
 *
 *  下标p表示通带(Pass), s表示阻带(Stop)
 *  Wp: 通带边界频率
 *  Ws: 阻带边界频率
 *  Rp: 通带边界频率对应的衰减，单位为db
 *  Rs: 阻带边界频率对应的衰减，单位为db
 */

void ButterworthLowpassFilter::Process() {
  if (filter_wp_ <= 0 || filter_ws_ <= 0||
      filter_rp_ <= 0 || filter_rs_ <= 0) return;
  filter_order_ = ProcessOrder(); 
  filter_wc_ = ProcessCutoffFrequency(
      filter_wp_, filter_rp_, GetFilterOrderAsInt());
}

double ButterworthLowpassFilter::ProcessOrder() {
  double Wp = 2 * PI * filter_wp_;
  double Ws = 2 * PI * filter_ws_;
  double Rp = filter_rp_;
  double Rs = filter_rs_;

  double val1 = pow(10, 0.1 * Rs) - 1;
  double val2 = pow(10, 0.1 * Rp) - 1;
  double val3 = log10(val1 / val2);

  double val4 = 2 * log10(Ws / Wp);
  return val3 / val4;
}

double ButterworthLowpassFilter::ProcessCutoffFrequency(
    double w, double r, int filter_order) {
  if (filter_order <= 0) return 0.0f;
  
  double a = pow(10, 0.1 * r) - 1;
  double b = -1.0 / (2 * filter_order);
  return 2 * PI * w * pow(a, b); 
}

int ButterworthLowpassFilter::GetFilterOrderAsInt() {
  return ceil(filter_order_);
}

double ButterworthLowpassFilter::GetFilterOrderAsDouble() {
  return filter_order_;
}

double ButterworthLowpassFilter::GetFilterCutoffFrequency() {
  return filter_wc_;
}

void ButterworthLowpassFilter::SetFilterWp(double value) {
  filter_wp_ = value;
}

void ButterworthLowpassFilter::SetFilterWs(double value) {
  filter_ws_ = value;
}

void ButterworthLowpassFilter::SetFilterRp(double value) {
  filter_rp_ = value;
}

void ButterworthLowpassFilter::SetFilterRs(double value) {
  filter_rs_ = value;
}

