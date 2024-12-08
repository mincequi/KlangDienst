import 'dart:math';

import 'package:KlangDienst/models/filter.dart';
import 'package:KlangDienst/services/filter_service.dart';
import 'package:get/get.dart';

import '../models/filter_model.dart';
import '../utils/freq_table.dart';

class FilterController extends GetxController {
  static var activeController = Rx<FilterController?>(null);

  bool isActive() => activeController.value == this;

  final filterService = Get.find<FilterService>();

  final model = FilterModel().obs;
  final response = List.filled(freqs.length, 0.0).obs;

  // Type
  var type = FilterType.Bypass.obs;
  void setFilterType(FilterType type_) {
    type.value = type_;
    switch (type_) {
      case FilterType.Bypass:
        freqEnabled.value = false;
        gainEnabled.value = false;
        qEnabled.value = false;
        break;
      case FilterType.Peaking:
      case FilterType.LowShelf:
      case FilterType.HighShelf:
        freqEnabled.value = true;
        gainEnabled.value = true;
        qEnabled.value = true;
        break;
      case FilterType.LowPass:
      case FilterType.HighPass:
        freqEnabled.value = true;
        gainEnabled.value = false;
        qEnabled.value = true;
        break;
    }
    _onFilterChanged();
  }

  // Frequency
  static const int _defaultFreqIdx = 72;
  final freqIdx = RxInt(_defaultFreqIdx);
  void incFreq() => freqIdx.value = min(freqIdx.value + 1, freqs.length - 5);
  void decFreq() => freqIdx.value = max(freqIdx.value - 1, 4);
  final freq = '1000'.obs;
  final freqUnit = 'Hz'.obs;
  final freqEnabled = false.obs;

  // Gain
  final gainIdx = RxInt(0);
  void incGain() => gainIdx.value = min(gainIdx.value + 1, 12);
  void decGain() => gainIdx.value = max(gainIdx.value - 1, -48);
  final gain = '0.0'.obs;
  final gainUnit = 'dB'.obs;
  final gainEnabled = false.obs;

  // Q Factor
  final qIdx = 12.obs;
  void decQ() => qIdx.value = min(qIdx.value + 1, 30);
  void incQ() => qIdx.value = max(qIdx.value - 1, 1);
  final q = RxDouble(_baseQ / 12.0);
  final qReadout = RxString((_baseQ / 12.0).toStringAsFixed(2));
  final qUnit = ''.obs;
  final qEnabled = false.obs;

  void _computeResponse() {
    activeController.value = this;
    Filter filter = Filter(
      type: type.value,
      freqIdx: freqIdx.value,
      gainIdx: gainIdx.value,
      q: q.value,
    );
    final response = filterService.response(filter);
    this.response.assignAll(response);
    this.response.refresh();
  }

  void _onFilterChanged() {
    model.value = FilterModel(
      type: type.value,
      freqIdx: freqIdx.value,
      gainIdx: gainIdx.value,
      qIdx: qIdx.value,
    );
    _computeResponse();
  }

  FilterController() {
    activeController.value = this;
    ever(freqIdx, (_) {
      double f = freqs[freqIdx.value];
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
      _onFilterChanged();
    });
    ever(gainIdx, (_) {
      // devide gain by 2 to get the actual gain value
      gain.value = (gainIdx.value * 0.5).toStringAsFixed(1);
      _onFilterChanged();
    });
    ever(qIdx, (_) {
      q.value = _baseQ / qIdx.value;
      qReadout.value = q.toStringAsFixed(2);
      _onFilterChanged();
    });
  }

  static final num _fractionalStep = pow(2, 1.0 / 6.0);
  static final double _baseQ = sqrt(_fractionalStep) / (_fractionalStep - 1);
}
