import 'package:flutter/material.dart';
import 'package:get/get.dart';

import '../controllers/eq_controller.dart';
import '../controllers/filter_manager_controller.dart';
import 'dynamic_grid_view.dart';
import 'widgets/eq_chart.dart';
import 'widgets/filter_card.dart';

class HomePage extends StatelessWidget {
  HomePage({super.key, required this.title});

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;
  var _counter = 0.obs;

  final FilterManagerController filtersManager =
      Get.put(FilterManagerController());

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        // TRY THIS: Try changing the color here to a specific color (to
        // Colors.amber, perhaps?) and trigger a hot reload to see the AppBar
        // change color while the other colors stay the same.
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(title),
      ),
      body: DynamicGridView(),
      /*Column(
        children: [DynamicGridView()],
      ),*/
      /*Column(
        children: [
          Padding(
              padding: const EdgeInsets.all(16.0),
              child: Obx(() {
                var controller = Get.find<EqController>();
                return EqChart(
                  frequencies: controller.frequencies,
                  gains: controller.gains.value,
                );
              }),
            ),
          DynamicGridView()
        ],*/
      floatingActionButton: FloatingActionButton(
        onPressed: () => _counter++,
        tooltip: 'Increment',
        child: const Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
