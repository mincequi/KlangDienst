import 'dart:math';

import 'package:KlangDienst/models/filter_coeffs.dart';
import 'package:KlangDienst/models/filter_model.dart';
import 'package:KlangDienst/utils/freq_table.dart';
import 'package:get/get.dart';

import '../models/filter_type.dart';
import '../models/loudness_coeffs.dart';

class FilterService extends GetxService {
  List<double> response(FilterModel filter) {
    List<FilterCoeffs> coeffsList;
    switch (filter.type) {
      case FilterType.Bypass:
      case FilterType.AllPass:
        return List.filled(freqs.length, 0.0);
      case FilterType.Loudness:
        //f1: 40, g1: 11, q1: 0.863695,
        //f2: 100, g2: 11, q2: 0.308462
        //f1: 12500, g1: 8, q1: 1.43949
        coeffsList = loudnessCoeffs(filter.freqIdx, filter.gainIdx);
      /*
        coeffsList = List.of([
          FilterCoeffs.fromModel(FilterModel(
              type: FilterType.Gain, gainIdx: -18.53 * filter.gainIdx / 20)),
          FilterCoeffs.fromModel(FilterModel(
              type: FilterType.Peaking,
              freqIdx: 16,
              gainIdx: 11 * filter.gainIdx / 20,
              qIdx: 10)),
          FilterCoeffs.fromModel(FilterModel(
              type: FilterType.Peaking,
              freqIdx: 32,
              gainIdx: 11 * filter.gainIdx / 20,
              qIdx: 28)),
          FilterCoeffs.fromModel(FilterModel(
              type: FilterType.Peaking,
              freqIdx: 116,
              gainIdx: 8 * filter.gainIdx / 20,
              qIdx: 6))
        ]);
        break;
      */
      default:
        coeffsList = List.of([FilterCoeffs.fromModel(filter)]);
        break;
    }

    List<double> magnitudeDb = List.filled(freqs.length, 0.0);
    for (var coeffs in coeffsList) {
      for (int i = 0; i < freqs.length; i++) {
        double frequency = freqs[i];
        double omega = 2 * pi * frequency / _sampleRate;
        // Complex numbers for calculating the response
        var numReal =
            coeffs.b0 + coeffs.b1 * cos(omega) + coeffs.b2 * cos(2 * omega);
        var numImag = coeffs.b1 * sin(omega) + coeffs.b2 * sin(2 * omega);
        var denReal =
            coeffs.a0 + coeffs.a1 * cos(omega) + coeffs.a2 * cos(2 * omega);
        var denImag = coeffs.a1 * sin(omega) + coeffs.a2 * sin(2 * omega);

        // Calculate the magnitude of H(omega)
        var magnitude = sqrt(numReal * numReal + numImag * numImag) /
            sqrt(denReal * denReal + denImag * denImag);

        // Convert magnitude to dB
        double db = 20 * log(magnitude) / ln10; // dB = 20 * log10(magnitude)
        magnitudeDb[i] += db;
      }
    }
    return magnitudeDb;
  }

  final double _sampleRate = 48000; // Default sample rate
}
