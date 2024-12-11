import 'dart:math';

import 'package:KlangDienst/models/filter_type.dart';

import '../utils/freq_table.dart';

class FilterModel {
  FilterType type = FilterType.Bypass;
  int freqIdx = 72;
  int gainIdx = 0;
  int qIdx = 12;

  double freq() => freqs[freqIdx];
  double gain() => gainIdx * 0.5;
  double q() => _baseQ / qIdx;

  FilterModel(
      {this.type = FilterType.Bypass,
      this.freqIdx = 72,
      this.gainIdx = 0,
      this.qIdx = 12});

  static final num _fractionalStep = pow(2, 1.0 / 6.0);
  static final double _baseQ = sqrt(_fractionalStep) / (_fractionalStep - 1);
}
