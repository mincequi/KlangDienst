import 'dart:developer' as developer;

import 'package:KlangDienst/services/eq_service.dart';
import 'package:KlangDienst/views/widgets/filter_card.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

class FilterGridPage extends StatelessWidget {
  const FilterGridPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final managerController = Get.find<EqService>();

    // Ensure controllers are initialized
    //managerController.initializeFilterControllers();

    // Screen width and spacing calculations
    final screenWidth = MediaQuery.of(context).size.width;
    const spacing = 0.0;

    return Obx(
      () {
        final filters = managerController.filters;
        var cardWidth = computeCardWidth(screenWidth);

        return Expanded(
          // This makes the GridView expand inside a Column.
          child: GridView.builder(
            padding: const EdgeInsets.all(spacing),
            gridDelegate: SliverGridDelegateWithMaxCrossAxisExtent(
              maxCrossAxisExtent: cardWidth,
              mainAxisExtent: _cardHeight,
              crossAxisSpacing: 0,
              mainAxisSpacing: 0,
            ),
            itemCount: filters.length,
            itemBuilder: (context, index) {
              // Get the FilterController for this specific filter using index
              final filterController = managerController.filters[index];

              // Pass the FilterController to the FilterCard
              return FilterCard(filterController: filterController);
            },
          ),
        );
      },
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
