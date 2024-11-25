import 'package:flutter/material.dart';

class DynamicGridView extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Builder(
      builder: (context) {
        // Get the width of the screen
        double screenWidth = MediaQuery.of(context).size.width;

        // Calculate the number of columns dynamically (for example, 200px per column)
        int columns = (screenWidth / 200).floor();
        if (columns < 1) columns = 1; // Ensure at least 1 column

        // Return GridView with dynamic column count
        return GridView.builder(
          padding: EdgeInsets.all(8),
          gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: columns, // Dynamic number of columns
            crossAxisSpacing: 8.0, // Horizontal space between items
            mainAxisSpacing: 8.0, // Vertical space between items
            childAspectRatio: 1.0, // Aspect ratio of each grid item
          ),
          itemCount: 20, // Number of items to display
          itemBuilder: (context, index) {
            return Card(
              elevation: 4,
              child: Center(
                child: Text('Item $index', style: TextStyle(fontSize: 18)),
              ),
            );
          },
        );
      },
    );
  }
}
