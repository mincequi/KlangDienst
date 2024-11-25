class FilterModel {
  double frequency; // Center frequency in Hz
  double gain; // Gain in dB
  double qFactor; // Quality factor (bandwidth)

  FilterModel({
    required this.frequency,
    required this.gain,
    required this.qFactor,
  });
}
