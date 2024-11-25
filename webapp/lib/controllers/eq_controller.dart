import 'package:get/get.dart';
import '../models/filter_model.dart';

class EqController extends GetxController {
  // Equalizer frequencies and gains (for chart visualization)
  final frequencies =
      <double>[20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000].obs;
  final gains =
      <double>[0, 0, 0, 0, 0, 0, 0, 0, 0, 0].obs; // Flat equalizer by default

  // List of filters
  final filters = <FilterModel>[
    FilterModel(frequency: 100, gain: 0, qFactor: 1),
    FilterModel(frequency: 1000, gain: 0, qFactor: 1),
    FilterModel(frequency: 10000, gain: 0, qFactor: 1),
  ].obs;

  // Updates the frequency of a specific filter
  void updateFilterFrequency(int index, double frequency) {
    if (index >= 0 && index < filters.length) {
      filters[index].frequency = frequency;
      filters.refresh(); // Triggers UI update
      _updateEqualizerGains(); // Recalculate EQ curve
    }
  }

  // Updates the gain of a specific filter
  void updateFilterGain(int index, double gain) {
    if (index >= 0 && index < filters.length) {
      filters[index].gain = gain;
      filters.refresh(); // Triggers UI update
      _updateEqualizerGains(); // Recalculate EQ curve
    }
  }

  // Updates the Q-factor of a specific filter
  void updateFilterQFactor(int index, double qFactor) {
    if (index >= 0 && index < filters.length) {
      filters[index].qFactor = qFactor;
      filters.refresh(); // Triggers UI update
      _updateEqualizerGains(); // Recalculate EQ curve
    }
  }

  // Recalculate the EQ gains based on filter settings
  void _updateEqualizerGains() {
    // Reset gains
    for (int i = 0; i < gains.length; i++) {
      gains[i] = 0;
    }

    // Apply each filter's effect to the gains
    for (final filter in filters) {
      for (int i = 0; i < frequencies.length; i++) {
        final freq = frequencies[i];
        final deltaGain = _calculateGainAtFrequency(filter, freq);
        gains[i] += deltaGain; // Combine gains
      }
    }

    // Clamp gains to avoid extreme values
    for (int i = 0; i < gains.length; i++) {
      gains[i] = gains[i].clamp(-12.0, 12.0); // Limit gain to [-12 dB, 12 dB]
    }

    gains.refresh(); // Update the UI
  }

  // Calculates the gain contribution of a single filter at a given frequency
  double _calculateGainAtFrequency(FilterModel filter, double frequency) {
    final freqRatio = frequency / filter.frequency;
    final bandwidth = 1.0 / filter.qFactor;
    final response = 1 / (1 + (freqRatio - 1).abs() / bandwidth);
    return filter.gain * response; // Scale by filter gain
  }
}
