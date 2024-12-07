import 'dart:math';

import 'package:KlangDienst/controllers/filter_controller.dart';
import 'package:KlangDienst/services/eq_service.dart';
import 'package:KlangDienst/utils/freq_table.dart';
import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:get/get.dart';

class EqChart extends StatelessWidget {
  final EqService _eqService = Get.find();

  EqChart({super.key});

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (context, constraints) {
        return /*Card(
          child:*/
            Container(
          width: constraints.maxWidth,
          height: constraints.maxWidth * 0.5,
          child: Obx(() => LineChart(
                duration: Duration.zero,
                LineChartData(
                  minX: 0,
                  maxX: freqs.length - 1,
                  minY: -24.0,
                  maxY: 6.0,
                  gridData: FlGridData(
                      show: true, horizontalInterval: 3, verticalInterval: 8),
                  titlesData: FlTitlesData(show: false),
                  borderData: FlBorderData(show: false),
                  lineBarsData: _lineChartBarData(context, _eqService.filters),
                ),
              )),
        );
      },
    );
  }

  List<LineChartBarData> _lineChartBarData(
      context, RxList<FilterController> filters) {
    List<LineChartBarData> data = [];
    // inactive filters
    for (var filter in filters) {
      if (!filter.isActive()) {
        data.add(LineChartBarData(
          spots: _spots(filter.response),
          dotData: const FlDotData(show: false),
          isCurved: false,
          color: Colors.white.withAlpha(127),
          barWidth: 1,
        ));
      }
    }
    // active filter
    for (var filter in filters) {
      if (filter.isActive()) {
        data.add(LineChartBarData(
          spots: _spots(filter.response),
          dotData: FlDotData(
            show: true, // Show dots for all points
            getDotPainter: (spot, percentage, barData, index) {
              // Only display a dot for the spot at index 2 (FlSpot(2, 5))
              if (index == filter.freqIdx.value) {
                return FlDotCirclePainter(
                  radius: 8, // Custom dot size
                  color: Theme.of(context).colorScheme.surface,
                  strokeWidth: _activeStrokeWidth,
                  strokeColor: Theme.of(context).colorScheme.primary,
                );
              }
              // Hide dots for all other spots
              return FlDotCirclePainter(
                radius: 0, // No dot shown for other points
              );
            },
          ),
          isCurved: false,
          color: Theme.of(context).colorScheme.primary,
          barWidth: _activeStrokeWidth,
        ));
      }
    }
    // sum of all filters
    data.add(LineChartBarData(
      spots: _spots(_eqService.response),
      dotData: const FlDotData(show: false),
      isCurved: false,
      color: Theme.of(context).colorScheme.primary.withAlpha(161),
      barWidth: 4,
    ));
    return data;
  }

  List<FlSpot> _spots(List<double> values) {
    return List.generate(
      values.length,
      (index) => FlSpot(index.toDouble(), values[index]),
    );
  }

  final double _activeStrokeWidth = 1.5;
}
