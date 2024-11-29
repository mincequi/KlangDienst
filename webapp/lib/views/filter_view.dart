import 'package:KlangDienst/controllers/filter_manager_controller.dart';
import 'package:KlangDienst/views/widgets/filter_card.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

class FilterView extends StatelessWidget {
  final FilterManagerController filtersManager = Get.find();

  @override
  Widget build(BuildContext context) {
    return Expanded(child: Builder(
      builder: (context) {
        // Get the width of the screen
        double screenWidth = MediaQuery.of(context).size.width;

        // Calculate the number of columns dynamically (for example, 200px per column)
        int columns = (screenWidth / 240).floor();
        if (columns < 1) columns = 1; // Ensure at least 1 column

        // Return GridView with dynamic column count
        return Obx(() => GridView.builder(
              gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                crossAxisCount: columns, // Dynamic number of columns
                crossAxisSpacing: 0.0, // Horizontal space between items
                mainAxisSpacing: 0.0, // Vertical space between items
                childAspectRatio: 1.4, // Aspect ratio of each grid item
              ),
              itemCount: filtersManager
                  .filterCount.value, // Number of items to display
              itemBuilder: (context, index) {
                return FilterCard(
                    controller: filtersManager
                        .filters[index]); // Return a FilterCard widget
                /*return Card(
              child: Center(
                child: Text('Item $index', style: TextStyle(fontSize: 18)),
              ),
            );*/
              },
            ));
      },
    ));
  }
}
