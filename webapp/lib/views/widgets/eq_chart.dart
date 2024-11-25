import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';

class EqChart extends StatelessWidget {
  final List<double> frequencies;
  final List<double> gains;

  EqChart({required this.frequencies, required this.gains});

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (context, constraints) {
        // Get the available width
        double width_ = constraints.maxWidth;
        // Calculate the height based on the width (e.g., 2 times the width)
        double height_ = width_ / 121 * 60; // Height is 2x the width
        return Card(
          child: Container(
            width: width_,
            height: height_,
            child: LineChart(
              LineChartData(
                //gridData: FlGridData(show: true),
                //titlesData: FlTitlesData(show: true),
                //borderData: FlBorderData(show: true),
                lineBarsData: [
                  LineChartBarData(
                    spots: List.generate(
                      frequencies.length,
                      (index) => FlSpot(frequencies[index], gains[index]),
                    ),
                    color: Colors.blue,
                    barWidth: 2,
                  ),
                ],
              ),
            ),
          ),
        );
      },
    );
  }
}
