import 'package:KlangDienst/models/filter_type.dart';

class FilterModel {
  FilterType type = FilterType.Bypass;
  int freqIdx = 72;
  int gainIdx = 0;
  int qIdx = 12;

  FilterModel(
      {this.type = FilterType.Bypass,
      this.freqIdx = 72,
      this.gainIdx = 0,
      this.qIdx = 12});
}
