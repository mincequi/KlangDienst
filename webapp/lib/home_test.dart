import 'dart:developer' as developer;

import 'package:KlangDienst/utils/freq_table.dart';
import 'package:KlangDienst/views/widgets/filter_param.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

import 'controllers/filter_manager_controller.dart';
import 'views/widgets/filter_card.dart';

class HomePageTest extends StatelessWidget {
  final FilterManagerController filterManager =
      Get.put(FilterManagerController());

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Dynamic GridView'),
      ),
      body: LayoutBuilder(
        builder: (context, constraints) {
          // Update the screen width in the controller
          //controller.updateScreenWidth(constraints.maxWidth);
          var cardWidth = computeCardWidth(constraints.maxWidth);
          return Obx(() {
            return GridView.builder(
              padding: const EdgeInsets.all(8),
              gridDelegate: SliverGridDelegateWithMaxCrossAxisExtent(
                maxCrossAxisExtent:
                    cardWidth, //controller.maxCardWidthObs.value,
                //maxCrossAxisExtent: constraints.maxWidth,
                mainAxisExtent: _cardHeight, // controller.cardHeight,
                crossAxisSpacing: 0,
                mainAxisSpacing: 0,
              ),
              //itemCount: controller.items.length,
              itemCount: filterManager.filterCount.value,
              itemBuilder: (context, index) {
                // Get the FilterController for this specific filter using index
                final filterController = filterManager.filters[index];
                return FilterCard(filterController: filterController);
              },
            );
          });
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: filterManager.addFilter,
        tooltip: 'Add Filter',
        child: Icon(Icons.add),
      ),
    );
  }

  double computeCardWidth(double screenWidth) {
    int cardCount = (screenWidth / _minCardWidth).floor();
    final calculatedWidth = screenWidth / cardCount;
    developer
        .log('screenWidth: $screenWidth, calculatedWidth: $calculatedWidth');
    return calculatedWidth;
  }

  final double _minCardWidth = 196;
  final double _cardHeight = 192;
}
