import 'dart:math';

import 'package:KlangDienst/models/filter_coeffs.dart';
import 'package:KlangDienst/models/filter_model.dart';
import 'package:KlangDienst/utils/freq_table.dart';
import 'package:get/get.dart';

import '../models/filter_type.dart';

class FilterService extends GetxService {
  List<double> response(FilterModel filter) {
    var coeffs = this.coeffs(filter);
    List<double> magnitudeDb = [];
    for (double frequency in freqs) {
      double omega = 2 * pi * frequency / _sampleRate;

      // Compute the transfer function H(omega)
      // H(omega) = (b0 + b1 e^(-j omega) + b2 e^(-j 2 omega)) / (a0 + a1 e^(-j omega) + a2 e^(-j 2 omega))

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
      magnitudeDb.add(db);
    }

    return magnitudeDb;
  }

  FilterCoeffs coeffs(FilterModel filter) {
    FilterCoeffs coeffs = FilterCoeffs();

    // Compute normalized frequency (omega_0)
    double frequency = filter.freq();
    double w0 = 2 * pi * frequency / _sampleRate;
    double cos_w0 = cos(w0);
    double alpha = sin(w0) / (2 * filter.q());
    num A = pow(
        10,
        filter.gainIdx /
            80.0); // Actually division by 40, but idx is in 2 dB steps

    switch (filter.type) {
      case FilterType.Peaking:
        // Peaking filter coefficients
        coeffs.b0 = 1 + alpha * A;
        coeffs.b1 = -2 * cos_w0;
        coeffs.b2 = 1 - alpha * A;
        coeffs.a0 = 1 + alpha / A;
        coeffs.a1 = -2 * cos_w0;
        coeffs.a2 = 1 - alpha / A;
        break;
      case FilterType.LowPass:
        // Low-pass filter coefficients
        coeffs.b0 = (1 - cos_w0) / 2;
        coeffs.b1 = 1 - cos_w0;
        coeffs.b2 = (1 - cos_w0) / 2;
        coeffs.a0 = 1 + alpha;
        coeffs.a1 = -2 * cos_w0;
        coeffs.a2 = 1 - alpha;
        break;
      case FilterType.HighPass:
        // High-pass filter coefficients
        coeffs.b0 = (1 + cos_w0) / 2;
        coeffs.b1 = -(1 + cos_w0);
        coeffs.b2 = (1 + cos_w0) / 2;
        coeffs.a0 = 1 + alpha;
        coeffs.a1 = -2 * cos_w0;
        coeffs.a2 = 1 - alpha;
        break;
      case FilterType.LowShelf:
        double alpha2 = 2 * sqrt(A) * alpha;
        coeffs.b0 = (A * ((A + 1) - (A - 1) * cos_w0 + alpha2));
        coeffs.b1 = (2 * A * ((A - 1) - (A + 1) * cos_w0));
        coeffs.b2 = (A * ((A + 1) - (A - 1) * cos_w0 - alpha2));
        coeffs.a0 = ((A + 1) + (A - 1) * cos_w0 + alpha2);
        coeffs.a1 = (-2 * ((A - 1) + (A + 1) * cos_w0));
        coeffs.a2 = ((A + 1) + (A - 1) * cos_w0 - alpha2);
        break;
      case FilterType.HighShelf:
        double alpha2 = 2 * sqrt(A) * alpha;
        coeffs.b0 = (A * ((A + 1) + (A - 1) * cos_w0 + alpha2));
        coeffs.b1 = (-2 * A * ((A - 1) + (A + 1) * cos_w0));
        coeffs.b2 = (A * ((A + 1) + (A - 1) * cos_w0 - alpha2));
        coeffs.a0 = ((A + 1) - (A - 1) * cos_w0 + alpha2);
        coeffs.a1 = (2 * ((A - 1) - (A + 1) * cos_w0));
        coeffs.a2 = ((A + 1) - (A - 1) * cos_w0 - alpha2);
        break;
      default:
        // Bypass or unsupported filter type (no coefficients)
        coeffs.b0 = 1.0;
        coeffs.a0 = 1.0;
        break;
    }

    // Normalize the coefficients (for stability)
    coeffs.b0 /= coeffs.a0;
    coeffs.b1 /= coeffs.a0;
    coeffs.b2 /= coeffs.a0;
    coeffs.a1 /= coeffs.a0;
    coeffs.a2 /= coeffs.a0;
    coeffs.a0 = 1.0;

    return coeffs;
  }

  final double _sampleRate = 48000; // Default sample rate
}
