import 'dart:math';

import 'package:KlangDienst/models/filter_type.dart';
import 'package:KlangDienst/services/filter_service.dart';
import 'package:get/get.dart';

import '../models/filter_model.dart';
import '../utils/freq_table.dart';

class FilterController extends GetxController {
  static var activeController = Rx<FilterController?>(null);
  bool isActive() => activeController.value == this;

  final _filterService = Get.find<FilterService>();

  FilterController() {
    activeController.value = this;
    _subscribeToFilterChanges();
  }

  FilterController.fromModel(FilterModel model) {
    activeController.value = this;
    type.value = model.type;
    freqIdx.value = model.freqIdx;
    gainIdx.value = model.gainIdx.toInt();
    qIdx.value = model.qIdx;
    _subscribeToFilterChanges();
  }

  void _subscribeToFilterChanges() {
    // We update first before subscribing
    _updateType();
    _updateFreq();
    _updateGain();
    _updateQ();
    _onFilterChanged();
    ever(type, (_) {
      _updateType();
      _onFilterChanged();
    });
    ever(freqIdx, (_) {
      _updateFreq();
      _onFilterChanged();
    });
    ever(gainIdx, (_) {
      _updateGain();
      _onFilterChanged();
    });
    ever(qIdx, (_) {
      _updateQ();
      _onFilterChanged();
    });
  }

  void _updateType() {
    switch (type.value) {
      case FilterType.Bypass:
        freqEnabled.value = false;
        gainEnabled.value = false;
        qEnabled.value = false;
        gainUnit.value = 'dB';
        break;
      case FilterType.Peaking:
      case FilterType.LowShelf:
      case FilterType.HighShelf:
        freqEnabled.value = true;
        gainEnabled.value = true;
        qEnabled.value = true;
        gainUnit.value = 'dB';
        break;
      case FilterType.LowPass:
      case FilterType.HighPass:
        freqEnabled.value = true;
        gainEnabled.value = false;
        qEnabled.value = true;
        gainUnit.value = 'dB';
        break;
      case FilterType.Loudness:
        freqEnabled.value = true;
        gainEnabled.value = true;
        qEnabled.value = false;
        // Round freqIndex to nearest of multiple of 4
        freqIdx.value = ((freqIdx.value / 4).round() * 4).clamp(16, 28);
        gainIdx.value = max(gainIdx.value, 0);
        gainUnit.value = 'phon';
        break;
      case FilterType.AllPass:
        freqEnabled.value = true;
        gainEnabled.value = false;
        qEnabled.value = true;
        gainUnit.value = 'dB';
        break;
      case FilterType.Gain:
        freqEnabled.value = false;
        gainEnabled.value = true;
        qEnabled.value = false;
        gainUnit.value = 'dB';
        break;
    }
    gainIdx.refresh();
  }

  void _updateFreq() {
    double f = freqs[freqIdx.value];
    if (f >= 2000) {
      f = f / 1000;
      freq.value = f.toStringAsFixed(1);
      freqUnit.value = 'kHz';
    } else if (f >= 100) {
      freq.value = f.toStringAsFixed(0);
      freqUnit.value = 'Hz';
    } else {
      freq.value = f.toStringAsFixed(1);
      freqUnit.value = 'Hz';
    }
  }

  void _updateGain() {
    if (type.value == FilterType.Loudness) {
      gain.value = gainIdx.value.toString();
      return;
    }
    gain.value = (gainIdx.value * 0.5).toStringAsFixed(1);
  }

  void _updateQ() {
    q.value = _baseQ / qIdx.value;
    qReadout.value = q.value.toStringAsFixed(2);
  }

  // This observable is currently only used to send the filter model to the server
  final model = FilterModel().obs;
  final response = List.filled(freqs.length, 0.0).obs;

  // Type
  var type = FilterType.Bypass.obs;
  void setFilterType(FilterType type_) {
    type.value = type_;
  }

  // Frequency
  static const int _defaultFreqIdx = 72;
  final freqIdx = RxInt(_defaultFreqIdx);
  void incFreq() {
    if (type.value == FilterType.Loudness) {
      freqIdx.value = min(freqIdx.value + 4, 28);
      return;
    }
    freqIdx.value = min(freqIdx.value + 1, freqs.length - 5);
  }

  void decFreq() {
    if (type.value == FilterType.Loudness) {
      freqIdx.value = max(freqIdx.value - 4, 16);
      return;
    }
    freqIdx.value = max(freqIdx.value - 1, 4);
  }

  final freq = '1000'.obs;
  final freqUnit = 'Hz'.obs;
  final freqEnabled = false.obs;

  // Gain
  final gainIdx = RxInt(0);
  void incGain() {
    if (type.value == FilterType.Loudness) {
      gainIdx.value = min(gainIdx.value + 1, 40);
      return;
    }
    gainIdx.value = min(gainIdx.value + 1, 12);
  }

  void decGain() {
    if (type.value == FilterType.Loudness) {
      gainIdx.value = max(gainIdx.value - 1, 0);
      return;
    }
    gainIdx.value = max(gainIdx.value - 1, -48);
  }

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
    var filter = FilterModel(
      type: type.value,
      freqIdx: freqIdx.value,
      gainIdx: gainIdx.value.toDouble(),
      qIdx: qIdx.value,
    );
    final response = _filterService.response(filter);
    this.response.assignAll(response);
    this.response.refresh();
  }

  void _onFilterChanged() {
    model.value = FilterModel(
      type: type.value,
      freqIdx: freqIdx.value,
      gainIdx: gainIdx.value.toDouble(),
      qIdx: qIdx.value,
    );
    _computeResponse();
  }

  static final num _fractionalStep = pow(2, 1.0 / 6.0);
  static final double _baseQ = sqrt(_fractionalStep) / (_fractionalStep - 1);
}
