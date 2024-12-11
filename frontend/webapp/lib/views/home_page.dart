import 'package:KlangDienst/models/filter_model.dart';
import 'package:KlangDienst/usecases/add_filter.dart';
import 'package:KlangDienst/views/filter_grid_view.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

import '../services/eq_service.dart';
import 'widgets/eq_chart.dart';

class HomePage extends StatelessWidget {
  HomePage({super.key, required this.title});

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
        onPressed: () =>
            AddFilter().call(FilterModel()), //_eqService.addFilter,
        tooltip: 'Add Filter',
        child: const Icon(Icons.add_card_rounded),
      ),
    );
  }
}
