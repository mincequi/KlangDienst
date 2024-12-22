import 'dart:math';

import 'package:KlangDienst/models/filter_model.dart';

import 'filter_type.dart';

class FilterCoeffs {
  double a0 = 1.0;
  double a1 = 0.0;
  double a2 = 0.0;
  double b0 = 1.0;
  double b1 = 0.0;
  double b2 = 0.0;

  static fromModel(FilterModel filter) {
    FilterCoeffs coeffs = FilterCoeffs();

    // Compute normalized frequency (omega_0)
    double frequency = filter.freq();
    double w0 = 2 * pi * frequency / _sampleRate;
    double cos_w0 = cos(w0);
    double alpha = sin(w0) / (2 * filter.q());
    num A = pow(
        10,
        filter.gainIdx /
            80.0); // Actually division by 40, but idx is in 0.5 dB steps

    switch (filter.type) {
      case FilterType.Gain:
        coeffs.b0 = pow(10, filter.gainIdx / 40.0)
            .toDouble(); // Actually division by 20, but idx is in 0.5 dB steps
        break;
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
      case FilterType.AllPass:
      case FilterType.Bypass:
      case FilterType.Loudness:
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

  static const double _sampleRate = 48000; // Default sample rate
}
