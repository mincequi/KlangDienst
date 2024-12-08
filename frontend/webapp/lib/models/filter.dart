enum FilterType {
  Bypass,
  Peaking,
  LowPass,
  HighPass,
  LowShelf,
  HighShelf,
}

class Filter {
  Filter(
      {required this.type,
      required this.freqIdx,
      required this.gainIdx,
      required this.q});

  FilterType type = FilterType.Bypass;
  int freqIdx = 68;
  int gainIdx = 0;
  double q = 1.0;
}
