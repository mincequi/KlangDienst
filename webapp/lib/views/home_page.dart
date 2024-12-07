import 'package:KlangDienst/views/dynamic_grid.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

import '../services/eq_service.dart';
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

  final EqService _eqService = Get.find();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
          padding: const EdgeInsets.all(0),
          child: Column(children: [
            EqChart(),
            const SizedBox(height: 4),
            const FilterGridView(),
          ])),
      floatingActionButton: FloatingActionButton(
        onPressed: _eqService.addFilter,
        tooltip: 'Add Filter',
        child: const Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
