import 'package:flutter/material.dart';

class FilterSlider extends StatelessWidget {
  final String label;
  final double frequency;
  final double gain;
  final double qFactor;
  final ValueChanged<double> onFrequencyChanged;
  final ValueChanged<double> onGainChanged;
  final ValueChanged<double> onQFactorChanged;

  const FilterSlider({
    required this.label,
    required this.frequency,
    required this.gain,
    required this.qFactor,
    required this.onFrequencyChanged,
    required this.onGainChanged,
    required this.onQFactorChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(label, style: const TextStyle(fontWeight: FontWeight.bold)),
        Slider(
          value: frequency,
          min: 20,
          max: 20000,
          onChanged: onFrequencyChanged,
          label: 'Frequency: ${frequency.toStringAsFixed(1)} Hz',
        ),
        Slider(
          value: gain,
          min: -12,
          max: 12,
          onChanged: onGainChanged,
          label: 'Gain: ${gain.toStringAsFixed(1)} dB',
        ),
        Slider(
          value: qFactor,
          min: 0.1,
          max: 10,
          onChanged: onQFactorChanged,
          label: 'Q-Factor: ${qFactor.toStringAsFixed(2)}',
        ),
        const Divider(),
      ],
    );
  }
}
