import 'package:flutter/material.dart';
import 'package:get/get.dart';
import '/controllers/filter_manager_controller.dart';
import '/views/widgets/filter_card.dart';

class FilterScreen extends StatelessWidget {
  final FilterManagerController filtersManager =
      Get.put(FilterManagerController());

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Dynamic Filters'),
        actions: [
          IconButton(
            icon: Icon(Icons.add),
            onPressed: filtersManager.addFilter, // Add a new filter
          ),
        ],
      ),
      body: Obx(() {
        return Padding(
          padding: const EdgeInsets.all(8.0),
          child: LayoutBuilder(
            builder: (context, constraints) {
              // Calculate how many columns fit based on available width
              int cardsPerRow = (constraints.maxWidth / 250).floor();
              if (cardsPerRow < 1) cardsPerRow = 1; // At least 1 column

              // GridView.builder with dynamic column count
              return GridView.builder(
                padding: EdgeInsets.all(8),
                gridDelegate: SliverGridDelegateWithMaxCrossAxisExtent(
                  maxCrossAxisExtent: 250,
                  mainAxisExtent: 120,
                  //crossAxisCount: cardsPerRow, // Dynamic column count
                  crossAxisSpacing: 8.0, // Horizontal space between items
                  mainAxisSpacing: 8.0, // Vertical space between items
                  childAspectRatio: 1.0, // Aspect ratio of each grid item
                ),
                itemCount:
                    filtersManager.filterCount.value, // Dynamic item count
                itemBuilder: (context, index) {
                  return FilterCard(
                      filterController: filtersManager.filters[index]);
                },
              );
            },
          ),
        );
      }),
    );
  }
}
