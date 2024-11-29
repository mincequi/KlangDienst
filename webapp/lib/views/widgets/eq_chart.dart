import 'dart:math';

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
        return Card(
          child: Container(
            width: constraints.maxWidth,
            height: constraints.maxWidth * 0.5,
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
