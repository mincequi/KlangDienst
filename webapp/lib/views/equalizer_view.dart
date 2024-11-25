import 'package:flutter/material.dart';
import 'package:get/get.dart';
import 'package:fl_chart/fl_chart.dart';
import '../controllers/eq_controller.dart';
import '../views/widgets/eq_chart.dart';
import '../views/widgets/filter_slider.dart';

class EqualizerView extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
        padding: const EdgeInsets.all(0.0),
        child: Obx(() {
          var controller = Get.find<EqController>();
          return EqChart(
            frequencies: controller.frequencies,
            gains: controller.gains.value,
          );
        }),
      ),
    );
  }
}
