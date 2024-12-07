import 'dart:math';

import 'package:KlangDienst/models/filter.dart';
import 'package:KlangDienst/services/filter_service.dart';
import 'package:get/get.dart';

import '../utils/freq_table.dart';

class FilterController extends GetxController {
  static var activeController = Rx<FilterController?>(null);

  final filterService = Get.find<FilterService>();

  var response = List.filled(121, 0.0).obs;

  // Type
  var type = FilterType.Bypass.obs;
  void setFilterType(FilterType type_) {
    type.value = type_;
    _computeResponse();
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

  void _computeResponse() {
    activeController.value = this;
    Filter filter = Filter(
      type: type.value,
      freqIdx: _freq.value,
      gain: _gain.value,
      q: _q.value,
    );
    final response = filterService.response(filter);
    this.response.assignAll(response);
    this.response.refresh();
  }

  FilterController() {
    activeController.value = this;
    ever(_freq, (_) {
      double f = freqs[_freq.value];
      if (f >= 2000) {
        f = f / 1000;
        freq.value = f.toStringAsFixed(1);
        freqUnit.value = 'kHz';
      } else if (f >= 50) {
        freq.value = f.toStringAsFixed(0);
        freqUnit.value = 'Hz';
      } else {
        freq.value = f.toStringAsFixed(1);
        freqUnit.value = 'Hz';
      }
      _computeResponse();
    });
    ever(_gain, (_) {
      gain.value = _gain.value.toStringAsFixed(1);
      _computeResponse();
    });
    ever(_q, (_) {
      if (_q.value >= 10) {
        q.value = _q.value.toStringAsFixed(1);
      } else {
        q.value = _q.value.toStringAsFixed(2);
      }
      _computeResponse();
    });
  }
}
