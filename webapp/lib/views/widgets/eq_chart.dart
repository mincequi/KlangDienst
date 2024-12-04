import 'dart:math';

import 'package:KlangDienst/services/eq_service.dart';
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
        return Card(
          child: Container(
            width: constraints.maxWidth,
            height: constraints.maxWidth * 0.5,
            child: Obx(() => LineChart(
                  duration: Duration.zero,
                  LineChartData(
                    minX: -1,
                    maxX: 121,
                    minY: -24.5,
                    maxY: 6.5,
                    gridData: FlGridData(
                        show: true, horizontalInterval: 3, verticalInterval: 8),
                    titlesData: FlTitlesData(show: false),
                    //borderData: FlBorderData(show: true),
                    lineBarsData: [
                      LineChartBarData(
                        spots: List.generate(
                          _eqService.response.value.length,
                          (index) => FlSpot(index.toDouble(),
                              _eqService.response.value[index]),
                          //gains.length,
                          //(index) => FlSpot(index.toDouble(), gains[index]),
                        ),
                        dotData: const FlDotData(show: false),
                        isCurved: false,
                        color: Colors.blue,
                        barWidth: 2,
                      ),
                    ],
                  ),
                )),
          ),
        );
      },
    );
  }
}
