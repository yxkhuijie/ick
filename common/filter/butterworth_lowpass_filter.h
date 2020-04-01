class ButterworthLowpassFilter {
 public:
  ButterworthLowpassFilter();
  ~ButterworthLowpassFilter();

 public:
  void Process();
  // calculate order (N) of the filter
  double ProcessOrder();
  double ProcessCutoffFrequency(double w, double r, int filter_order);

  int GetFilterOrderAsInt();
  double GetFilterOrderAsDouble();
  double GetFilterCutoffFrequency();
  void SetFilterWp(double value);
  void SetFilterWs(double value);
  void SetFilterRp(double value);
  void SetFilterRs(double value);

 private:
  double filter_order_;

  // filter passband frequency(HZ)
  double filter_wp_;
  // filter stopband frequency(HZ)
  double filter_ws_;
  // double passband attenuation gain(db)
  double filter_rp_;
  // double stopband attenuation gain(db)
  double filter_rs_;

  // double cutoff freqyency(HZ)
  double filter_wc_;
};
