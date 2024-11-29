import 'package:KlangDienst/controllers/filter_manager_controller.dart';
import 'package:KlangDienst/views/widgets/filter_card.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

class FilterGridPage extends StatelessWidget {
  const FilterGridPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final managerController = Get.find<FilterManagerController>();

    // Ensure controllers are initialized
    //managerController.initializeFilterControllers();

    // Screen width and spacing calculations
    final screenWidth = MediaQuery.of(context).size.width;
    const spacing = 0.0;
    const cardMinWidth = 150.0;
    final columnCount = (screenWidth / (cardMinWidth + spacing)).floor();

    return Obx(
      () {
        final filters = managerController.filters;

        return Expanded(
          // This makes the GridView expand inside a Column.
          child: GridView.builder(
            padding: const EdgeInsets.all(spacing),
            gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
              crossAxisCount: columnCount,
              crossAxisSpacing: spacing,
              mainAxisSpacing: spacing,
            ),
            itemCount: filters.length,
            itemBuilder: (context, index) {
              final filter = filters[index];

              // Get the FilterController for this specific filter using index
              final filterController = managerController.filters[index];

              // Pass the FilterController to the FilterCard
              return FilterCard(controller: filterController);
            },
          ),
        );
      },
    );
  }
}
