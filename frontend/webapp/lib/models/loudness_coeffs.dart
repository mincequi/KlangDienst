import 'filter_coeffs.dart';
import 'filter_model.dart';
import 'filter_type.dart';

// { f1: 40.00, g1: 11.50, q1: 1.08, f2: 100.00, g2: 11.50, q2: 0.33 } maxDeviation: 0.49, rms: 0.27, subsonic: 16.55 dB
// { f1: 50.00, g1: 8.50,  q1: 1.44, f2: 100.00, g2: 12.00, q2: 0.35 } maxDeviation: 0.48, rms: 0.26, subsonic: 15.74 dB
// { f1: 63.00, g1: 9.00,  q1: 1.44, f2: 125.00, g2: 10.50, q2: 0.39 } maxDeviation: 0.46, rms: 0.25, subsonic: 14.84 dB
// { f1: 80.00, g1: 9.00,  q1: 1.44, f2: 160.00, g2:  9.00, q2: 0.43 } maxDeviation: 0.48, rms: 0.25, subsonic: 13.82 dB

List<FilterCoeffs> loudnessCoeffs(int fIndex, double gIndex) {
  switch (fIndex) {
    case 16:
      return List.of([
        FilterCoeffs.fromModel(
            FilterModel(type: FilterType.Gain, gainIdx: -19.00 * gIndex / 20)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 16,
            gainIdx: 11.5 * gIndex / 20,
            qIdx: 8)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 32,
            gainIdx: 11.5 * gIndex / 20,
            qIdx: 26)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 116,
            gainIdx: 7.5 * gIndex / 20,
            qIdx: 6))
      ]);
    case 20:
      return List.of([
        FilterCoeffs.fromModel(
            FilterModel(type: FilterType.Gain, gainIdx: -17.6 * gIndex / 20)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 20,
            gainIdx: 8.5 * gIndex / 20,
            qIdx: 6)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 32,
            gainIdx: 12 * gIndex / 20,
            qIdx: 25)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 116,
            gainIdx: 7.5 * gIndex / 20,
            qIdx: 6))
      ]);
    case 24:
      return List.of([
        FilterCoeffs.fromModel(
            FilterModel(type: FilterType.Gain, gainIdx: -16.63 * gIndex / 20)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 24,
            gainIdx: 9.0 * gIndex / 20,
            qIdx: 6)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 36,
            gainIdx: 10.5 * gIndex / 20,
            qIdx: 22)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 116,
            gainIdx: 7.5 * gIndex / 20,
            qIdx: 6))
      ]);
    case 28:
      return List.of([
        FilterCoeffs.fromModel(
            FilterModel(type: FilterType.Gain, gainIdx: -15.21 * gIndex / 20)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 28,
            gainIdx: 9.0 * gIndex / 20,
            qIdx: 6)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 40,
            gainIdx: 9.0 * gIndex / 20,
            qIdx: 20)),
        FilterCoeffs.fromModel(FilterModel(
            type: FilterType.Peaking,
            freqIdx: 116,
            gainIdx: 7.5 * gIndex / 20,
            qIdx: 6))
      ]);
    default:
      return List.empty();
  }
}
