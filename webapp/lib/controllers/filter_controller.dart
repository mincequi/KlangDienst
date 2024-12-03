import 'dart:math';

import 'package:get/get.dart';

import '../utils/freq_table.dart';

// Controller to manage the states
// FilterController for individual filter settings
class FilterController extends GetxController {
  var type = 'LowPass'.obs;
  void setFilterType(String type_) {
    type.value = type_;
  }

  // Frequency
  final _freq = 68.obs;
  void incFreq() => _freq.value = min(_freq.value + 1, 120);
  void decFreq() => _freq.value = max(_freq.value - 1, 0);
  final freq = '1000'.obs;
  final freqUnit = 'Hz'.obs;

  // Gain
  final _gain = 0.0.obs;
  void incGain() => _gain.value = min(_gain.value + 0.5, 12);
  void decGain() => _gain.value = max(_gain.value - 0.5, -24);
  final gain = '0.0'.obs;
  final gainUnit = 'dB'.obs;

  // Q Factor
  final _q = 1.0.obs;
  void incQ() => _q.value = min(_q.value + 0.1, 12);
  void decQ() => _q.value = max(_q.value - 0.1, 0.1);
  final q = '1.0'.obs;
  final qUnit = ''.obs;

  FilterController() {
    ever(_freq, (_) {
      double f = freqs[_freq.value];
      /*if (f >= 15000) {
        f = f / 1000;
        freq.value = f.toStringAsFixed(0);
        freqUnit.value = 'kHz';
      } else*/
      if (f >= 2000) {
        f = f / 1000;
        freq.value = f.toStringAsFixed(1);
        freqUnit.value = 'kHz';
      } else {
        f = f.roundToDouble();
        freq.value = f.toStringAsFixed(0);
        freqUnit.value = 'Hz';
      }
    });
    ever(_gain, (_) {
      gain.value = _gain.value.toStringAsFixed(1);
    });
    ever(_q, (_) {
      if (_q.value >= 10) {
        q.value = _q.value.toStringAsFixed(1);
      } else {
        q.value = _q.value.toStringAsFixed(2);
      }
    });
  }
}
